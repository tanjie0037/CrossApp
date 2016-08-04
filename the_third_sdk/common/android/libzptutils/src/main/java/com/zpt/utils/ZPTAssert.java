package com.zpt.utils;

/**
 * Created by tanjie on 16/8/2.
 */
public class ZPTAssert {
    private static boolean _debug = false;

    public static void init(boolean debug) {
        _debug = debug;
    }

    public static void check(boolean expr) {
        check(expr, "assert failed.");
    }

    public static void check(boolean expr, String desc) {
        if (!expr && _debug) {
            ZPTLog.d(desc);
            throw new RuntimeException();
        }
    }
}
