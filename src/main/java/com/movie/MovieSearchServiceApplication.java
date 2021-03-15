package com.movie;

import org.apache.thrift.transport.TTransportException;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;


@SpringBootApplication
public class MovieSearchServiceApplication {

	public static void main(String[] args) {
		System.out.println("Movie search server ");
		MovieSearchServer server = new MovieSearchServer();
        try {
			server.start();
		} catch (TTransportException e) {
			e.printStackTrace();
		}
	}

}
