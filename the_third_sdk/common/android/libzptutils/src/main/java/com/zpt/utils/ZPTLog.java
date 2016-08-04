package com.zpt.utils;

import android.util.Log;

public class ZPTLog {
	private static boolean enable = true;
	private static String TAG_DEFAULT = "---zpt";

	public static void init(boolean debug, String tag) {
		enable = debug;
		TAG_DEFAULT = tag;
	}

	public static void v(String tag, String msg){
		if(enable){
			Log.v(tag, msg);
		}
	}
	
	public static void d(String tag, String msg){
		if(enable){
			Log.d(tag, msg);
		}
	}
	
	public static void e(String tag, String msg){
		if(enable){
			Log.e(tag, msg);
		}
	}
	
	public static void v(String msg){
		if(enable){
			Log.v(TAG_DEFAULT, msg);
		}
	}
	
	public static void d(String msg){
		if(enable){
			Log.d(TAG_DEFAULT, msg);
		}
	}
	
	public static void e(String msg){
		if(enable){
			Log.e(TAG_DEFAULT, msg);
		}
	}
}
