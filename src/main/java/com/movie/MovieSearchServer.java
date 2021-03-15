package com.movie;

import com.thrift.*;

import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TSimpleServer;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TServerTransport;
import org.apache.thrift.transport.TTransportException;

public class MovieSearchServer {

    private TServer server;

    public void start() throws TTransportException {
        TServerTransport serverTransport = new TServerSocket(9098);
        server = new TSimpleServer(new TServer.Args(serverTransport)
            .processor(new MovieSearchService.Processor<>(new MovieSearchServiceImpl())));

        System.out.print("Starting the Movie search server..... ");

        server.serve();

        System.out.println("done.");
    }

    public void stop() {
        if (server != null && server.isServing()) {
            System.out.print("Stopping the server... ");

            server.stop();

            System.out.println("done.");
        }
    }
}
