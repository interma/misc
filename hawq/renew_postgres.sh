set -x
hawq stop cluster --mode=immediate -a
cp ../incubator-hawq/src/backend/postgres bin
hawq start cluster
