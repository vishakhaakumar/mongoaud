package com.movie;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

import com.thrift.*;

public class MovieSearchClient {
		public static void main(String[] args) throws TException {
			
		    TTransport transport = new TSocket("localhost", 9098);
	        try {
	            transport.open();
	            System.out.println("Inside the Movie search Client ----> ");
	            TProtocol protocol = new TBinaryProtocol(transport);
	            
	            MovieSearchService.Client client = new MovieSearchService.Client(protocol);
	            System.out.println("Movie found is  = " + client.getMovie("showman"));
	        }catch(Exception e){
	        	e.printStackTrace();
	        	System.out.println("error in client");
	        }
	        finally {
	            transport.close();
	        }
	    }
}

