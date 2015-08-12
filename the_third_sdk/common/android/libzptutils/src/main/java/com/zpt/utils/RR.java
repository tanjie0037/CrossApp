package com.zpt.utils;

import java.lang.reflect.Field;

public class RR {
	private static String packageName = null;
	private static boolean checkedDebugStatus = false;
	private static boolean isDebug = false;

	public static void init(String pName) {
		packageName = pName;
	}
	
	public static int get(String type, String key) {
		if (packageName == null) {
			assert(false);
			System.out.println("should init this!");
			return 0;
		}
		
		Field filed;
		
		try {
			Class<?> clazz = Class.forName(packageName + ".R$" + type);
			filed = clazz.getDeclaredField(key);
			filed.setAccessible(true);

			return filed.getInt(clazz.newInstance());
			
		} catch (Exception e) {
			e.printStackTrace();
			assert(false);
			return 0;
		}
	}

	public static int anim(String key) {
		return get("anim", key);
	}

	public static int attr(String key) {
		return get("attr", key);
	}

	public static int color(String key) {
		return get("color", key);
	}

	public static int drawable(String key) {
		return get("drawable", key);
	}

	public static int id(String key) {
		return get("id", key);
	}

	public static int layout(String key) {
		return get("layout", key);
	}

	public static int raw(String key) {
		return get("raw", key);
	}

	public static int string(String key) {
		return get("string", key);
	}

	public static boolean debug() {
		if (checkedDebugStatus) {
			return isDebug;
		}

		checkedDebugStatus = true;

		if (packageName == null) {
			assert(false);
			System.out.println("should init this!");
			isDebug = true;
			return true;
		}
		
		Field field;
		
		try {
			Class<?> clazz = Class.forName(packageName + ".BuildConfig");
			field = clazz.getDeclaredField("DEBUG");
			field.setAccessible(true);

			isDebug = field.getBoolean(clazz.newInstance());
			
		} catch (Exception e) {
			e.printStackTrace();
			assert(false);
			isDebug = true;
		}

		return isDebug;
	}

	public static void setDebug(boolean debug) {
		checkedDebugStatus = true;
		isDebug = debug;
	}
}