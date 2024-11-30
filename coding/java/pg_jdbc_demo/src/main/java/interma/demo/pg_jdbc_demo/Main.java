package interma.demo.pg_jdbc_demo;

import java.sql.*;
import java.util.*;

/**
 * Created by hma on 17/03/2017.
 */
public class Main {

    /*
     */
    public static void main(String[] argv) throws Exception {
        String auth = System.getProperty("auth", "simple");

        if (auth.equals("kerberos"))
            //test_kerberos_pg();
            test_kerberos_pg2();
        else
            test_pg();
        System.out.printf("demo finished\n");
    }


    private static boolean get_driver() {
        System.out.println("-------- PostgreSQL "
                + "JDBC Connection Testing ------------");

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("Where is your PostgreSQL JDBC Driver? "
                    + "Include in your library path!");
            e.printStackTrace();
            return false;
        }
        System.out.println("PostgreSQL JDBC Driver Registered!");
        return true;
    }

    //tested on mac
    //    run: java -Dport=15432 -jar /Users/hma/git/misc/java/pg_jdbc_demo/target/pg_jdbc_demo-1.0-SNAPSHOT.jar
    private static void test_pg() {
        if (!get_driver())
            return;

        String port = System.getProperty("port", "5432");
        String user = System.getProperty("user", "hma");
        String pass = System.getProperty("pass", "");

        String conn_url = String.format("jdbc:postgresql://127.0.0.1:%s/postgres", port);
        System.out.println( String.format("conn_url[%s], user[%s], pass[%s]", conn_url, user, pass) );

        Connection connection = null;
        try {
            connection = DriverManager.getConnection(conn_url, user, pass);
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You made it, take control your database now!");
        } else {
            System.out.println("Failed to make connection!");
        }
    }

    /*
    tested on pulse
    ~/.java.login.config
    pgjdbc {
        com.sun.security.auth.module.Krb5LoginModule required
        doNotPrompt=false
        debug=true
        client=true;
    };

        run: /usr/java/jdk1.7.0_71/jre/bin/java -Dauth=kerberos -jar pg_jdbc_demo-1.0-SNAPSHOT.jar
     */
    private static void test_kerberos_pg() {
        if (!get_driver())
            return;

        String user = System.getProperty("user", "interma");
        String pass = System.getProperty("pass", "welcome1");

        System.out.println("kerberos mode");
        //jdbc:postgresql://mdw:5432/mytest?kerberosServerName=postgres&jaasApplicationName=pgjdbc&user=gpadmin/kerberos-gpdb
        Properties props = new Properties();
        props.setProperty("user", user);
        props.setProperty("password", pass);
        props.setProperty("kerberosServerName", "postgres");
        props.setProperty("jaasApplicationName", "pgjdbc");


        Connection connection = null;
        try {
            connection = DriverManager.getConnection("jdbc:postgresql://test1:21000/postgres", props);
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You made it, take control your database now!");
        } else {
            System.out.println("Failed to make connection!");
        }

        //do some sqls
        try
        {
            Statement st = connection.createStatement();
            ResultSet rs = st.executeQuery("SELECT i FROM test");
            while ( rs.next() )
            {
                int id = rs.getInt    ("i");
                System.out.println( String.format("\trow: %d", id) );
            }
            rs.close();
            st.close();
        }
        catch (SQLException se) {
            System.err.println("Threw a SQLException creating the list of blogs.");
            System.err.println(se.getMessage());
        }
    }

	//tested on local vm
	private static void test_kerberos_pg2() {
        if (!get_driver())
            return;

        String user = System.getProperty("user", "interma");
        String pass = System.getProperty("pass", "welcome1");

        System.out.println("kerberos mode");
		System.setProperty("java.security.auth.login.config","/tmp/.java.login.config.bak");
        //jdbc:postgresql://mdw:5432/mytest?kerberosServerName=postgres&jaasApplicationName=pgjdbc&user=gpadmin/kerberos-gpdb
        Properties props = new Properties();
        props.setProperty("user", user);
        props.setProperty("password", pass);
        props.setProperty("kerberosServerName", "postgres");
        props.setProperty("jaasApplicationName", "pgjdbc");


        Connection connection = null;
        try {
            connection = DriverManager.getConnection("jdbc:postgresql://c6402.ambari.apache.org:5432/postgres", props);
        } catch (SQLException e) {
            System.out.println("Connection Failed! Check output console");
            e.printStackTrace();
            return;
        }

        if (connection != null) {
            System.out.println("You made it, take control your database now!");
        } else {
            System.out.println("Failed to make connection!");
        }
	}

}
