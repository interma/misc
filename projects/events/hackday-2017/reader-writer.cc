// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the License for the
// specific language governing permissions and limitations
// under the License.

#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>

#include <arrow/io/file.h>

#include <parquet/api/reader.h>
#include <parquet/api/writer.h>

/*
 * This example describes writing and reading Parquet Files in C++ and serves as a
 * reference to the API.
 * The file contains all the physical data types supported by Parquet.
 **/

/* Parquet is a structured columnar file format
 * Parquet File = "Parquet data" + "Parquet Metadata"
 * "Parquet data" is simply a vector of RowGroups. Each RowGroup is a batch of rows in a
 * columnar layout
 * "Parquet Metadata" contains the "file schema" and attributes of the RowGroups and their
 * Columns
 * "file schema" is a tree where each node is either a primitive type (leaf nodes) or a
 * complex (nested) type (internal nodes)
 * For specific details, please refer the format here:
 * https://github.com/apache/parquet-format/blob/master/LogicalTypes.md
 **/

//constexpr int NUM_ROWS_PER_ROW_GROUP = 500;
//constexpr int NUM_ROWS_PER_ROW_GROUP = 246723;
constexpr int FIXED_LENGTH = 10;
//const char PARQUET_FILENAME[] = "parquet_cpp_example.parquet";
const char PARQUET_FILENAME[] = "interma_example.parquet";

using parquet::Repetition;
using parquet::Type;
using parquet::LogicalType;
using parquet::schema::PrimitiveNode;
using parquet::schema::GroupNode;



static std::shared_ptr<GroupNode> SetupSchema2() {
	parquet::schema::NodeVector fields;

	fields.push_back(PrimitiveNode::Make(
			"id", Repetition::REQUIRED, Type::INT64, LogicalType::NONE));
	fields.push_back(PrimitiveNode::Make(
			"name", Repetition::REQUIRED, Type::BYTE_ARRAY, LogicalType::NONE));
	fields.push_back(PrimitiveNode::Make(
			"price", Repetition::REQUIRED, Type::INT32, LogicalType::NONE));
	fields.push_back(PrimitiveNode::Make(
			"ts", Repetition::REQUIRED, Type::INT32, LogicalType::NONE));

	// Create a GroupNode named 'schema' using the primitive nodes defined above
	// This GroupNode is the root node of the schema tree
	return std::static_pointer_cast<GroupNode>(
		GroupNode::Make("schema", Repetition::REQUIRED, fields));
}

int main(int argc, char** argv) {
	/**********************************************************************************
	  PARQUET READER EXAMPLE
	 **********************************************************************************/
	try {
		// Create a local file output stream instance.
		using FileClass = ::arrow::io::FileOutputStream;
		std::shared_ptr<FileClass> out_file;
		PARQUET_THROW_NOT_OK(FileClass::Open(PARQUET_FILENAME, &out_file));

		// Setup the parquet schema
		std::shared_ptr<GroupNode> schema = SetupSchema2();

		// Add writer properties
		parquet::WriterProperties::Builder builder;
		std::shared_ptr<parquet::WriterProperties> props = builder.build();

		// Create a ParquetFileWriter instance
		std::shared_ptr<parquet::ParquetFileWriter> file_writer =
			parquet::ParquetFileWriter::Open(out_file, schema, props);


		/*****/

		// Create a ParquetReader instance
		std::unique_ptr<parquet::ParquetFileReader> parquet_reader =
			parquet::ParquetFileReader::OpenFile(argv[1], false);

		// Get the File MetaData
		std::shared_ptr<parquet::FileMetaData> file_metadata = parquet_reader->metadata();

		// Get the number of RowGroups
		int num_row_groups = file_metadata->num_row_groups();

		// Get the number of Columns
		int num_columns = file_metadata->num_columns();
		assert(num_columns == 4);

		// Iterate over all the RowGroups in the file
		for (int r = 0; r < num_row_groups; ++r) {
			// Get the RowGroup Reader
			std::shared_ptr<parquet::RowGroupReader> row_group_reader =
				parquet_reader->RowGroup(r);

			int64_t row_count = row_group_reader->metadata()->num_rows();
			std::cout<<r<<" "<<row_count<<std::endl;

			int64_t values_read = 0;
			int64_t rows_read = 0;
			int16_t definition_level;
			int16_t repetition_level;
			int i;
			std::shared_ptr<parquet::ColumnReader> column_reader;

			// Append a RowGroup with a specific number of rows.
			parquet::RowGroupWriter* rg_writer =
				file_writer->AppendRowGroup(row_count);

			// Get the Column Reader for the Int64 column
			column_reader = row_group_reader->Column(0);
			parquet::Int64Reader* int64_reader =
				static_cast<parquet::Int64Reader*>(column_reader.get());

			parquet::Int64Writer* int64_writer =
				static_cast<parquet::Int64Writer*>(rg_writer->NextColumn());
			// Read all the rows in the column
			i = 0;
			while (int64_reader->HasNext()) {
				int64_t value;
				// Read one value at a time. The number of rows read is returned. values_read
				// contains the number of non-null rows
				rows_read = int64_reader->ReadBatch(
					1, &definition_level, &repetition_level, &value, &values_read);
				// Ensure only one value is read
				assert(rows_read == 1);
				// There are no NULL values in the rows written
				assert(values_read == 1);

				//std::cout<<"id:"<<value<<std::endl;
				int64_writer->WriteBatch(1, nullptr, nullptr, &value);

				i++;
			}


			// Get the Column Reader for the ByteArray column
			column_reader = row_group_reader->Column(1);
			parquet::ByteArrayReader* ba_reader =
				static_cast<parquet::ByteArrayReader*>(column_reader.get());
			parquet::ByteArrayWriter* ba_writer =
				static_cast<parquet::ByteArrayWriter*>(rg_writer->NextColumn());
			// Read all the rows in the column
			i = 0;
			while (ba_reader->HasNext()) {
				parquet::ByteArray value;
				// Read one value at a time. The number of rows read is returned. values_read
				// contains the number of non-null rows
				rows_read =
					ba_reader->ReadBatch(1, &definition_level, nullptr, &value, &values_read);
				// Ensure only one value is read
				assert(rows_read == 1);


				ba_writer->WriteBatch(1, nullptr, nullptr, &value);

				i++;
			}

			// Get the Column Reader for the Int32 column
			column_reader = row_group_reader->Column(2);
			parquet::Int32Reader* int32_reader =
				static_cast<parquet::Int32Reader*>(column_reader.get());
			parquet::Int32Writer* int32_writer =
				static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
			// Read all the rows in the column
			i = 0;
			while (int32_reader->HasNext()) {
				int32_t value;
				// Read one value at a time. The number of rows read is returned. values_read
				// contains the number of non-null rows
				rows_read = int32_reader->ReadBatch(1, nullptr, nullptr, &value, &values_read);
				// Ensure only one value is read
				assert(rows_read == 1);
				// There are no NULL values in the rows written
				assert(values_read == 1);


				// Write the Int32 column
				int32_writer->WriteBatch(1, nullptr, nullptr, &value);

				i++;
			}

			// Get the Column Reader for the Int32 column
			column_reader = row_group_reader->Column(3);
			parquet::Int32Reader* int32_reader2 =
				static_cast<parquet::Int32Reader*>(column_reader.get());
			parquet::Int32Writer* int32_writer2 =
				static_cast<parquet::Int32Writer*>(rg_writer->NextColumn());
			// Read all the rows in the column
			i = 0;
			while (int32_reader2->HasNext()) {
				int32_t value;
				// Read one value at a time. The number of rows read is returned. values_read
				// contains the number of non-null rows
				rows_read = int32_reader2->ReadBatch(1, nullptr, nullptr, &value, &values_read);
				// Ensure only one value is read
				assert(rows_read == 1);
				// There are no NULL values in the rows written
				assert(values_read == 1);

				// Write the Int32 column
				int32_writer2->WriteBatch(1, nullptr, nullptr, &value);

				i++;
			}

		}

		// Close the ParquetFileWriter
		file_writer->Close();

		// Write the bytes to file
		out_file->Close();

	} catch (const std::exception& e) {
		std::cerr << "Parquet read error: " << e.what() << std::endl;
		return -1;
	}

	return 0;


	std::cout << "Parquet Writing and Reading Complete" << std::endl;

	return 0;
}

