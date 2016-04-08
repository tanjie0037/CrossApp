package com.zpt.libsuperrewards;

import android.app.Activity;
import com.playerize.superrewards.SuperRewards;

/**
 * Created by tanjie on 15/8/15.
 */
public class SuperrewardsWrapper {
    private static String _uId = "";
    private static String _appHash = "";
    private static Activity _ctx = null;

    public static void init(Activity act, String uId, String appHash) {
        _ctx = act;
        _uId = uId;
        _appHash = appHash;
    }

    public static void showOfferwall() {
        SuperRewards sr = new SuperRewards(_ctx.getResources(), _ctx.getPackageName());
        sr.showOffers(_ctx, _appHash, _uId);
    }
}
