package com.hisilicon.proxyservice;

public class ProxyService {
	
	private LogViewListener log_view_listener;
	
	public void setListViewListener(LogViewListener l)
	{
		log_view_listener = l;
	}
	
	public void log(String m)
	{
		/*
		 *  here need to be protected in multi-thread.
		 *  but now, just do this. modify it later...
		 */
		log_view_listener.printOutput(m);
	}
	
	public native int startService();
	public native int stopService();
	public native int init();
	
	public void load() {
	
		try
		{		
			System.loadLibrary("proxyservice");
		}
		catch(Exception e) 
		{
			e.printStackTrace(System.out);
		}
		
		log("Load proxyservice OK!");
		
	}	
}
