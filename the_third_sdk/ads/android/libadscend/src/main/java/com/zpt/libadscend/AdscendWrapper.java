package com.zpt.libadscend;

import android.content.Context;
import android.content.Intent;

import com.adscendmedia.sdk.ui.OffersActivity;

import java.util.Hashtable;

/**
 * Created by tanjie on 15/8/14.
 */
public class AdscendWrapper {
    private static Context _ctx = null;
    private static String _publisherId = "";
    private static String _adwallId = "";
    private static String _userId = "";

    public static void init(Context ctx, String userId, String publisherId, String adwallId) {
        _ctx = ctx;
        _publisherId = publisherId;
        _adwallId = adwallId;
        _userId = userId;
    }

    public static void showOfferwall() {
        Intent intent = OffersActivity.getIntentForOfferWall(_ctx, _publisherId, _adwallId, _userId);
        _ctx.startActivity(intent);
    }
}
