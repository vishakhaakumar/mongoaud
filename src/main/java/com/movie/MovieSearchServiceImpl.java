package com.movie;

import org.apache.thrift.TException;

import com.thrift.*;

public class MovieSearchServiceImpl implements MovieSearchService.Iface {

	@Override
	public String getMovie(String movieName) throws TException {
		//System.out.println(" The Movie found is ------------>>"+movieName);
		return "The Showman Business";
	}

}
