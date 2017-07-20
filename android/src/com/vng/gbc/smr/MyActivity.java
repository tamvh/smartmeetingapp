package com.vng.gbc.smr;

import android.view.LayoutInflater;
import android.view.View;
import android.content.Intent;
import android.widget.Toast;

import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import android.os.ParcelUuid;

import android.os.Bundle;
import android.os.Build;
import android.util.Log;

//import io.fabric.sdk.android.Fabric;
//import com.crashlytics.android.Crashlytics;
//import com.crashlytics.android.ndk.CrashlyticsNdk;

public class MyActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private final String TAG = MyActivity.class.getSimpleName();

    //private View decorView;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        final Fabric fabric = new Fabric.Builder(this)
//                .kits(new Crashlytics(), new CrashlyticsNdk())
//                .debuggable(true)
//                .build();
        //Fabric.with(this, new Crashlytics(), new CrashlyticsNdk());
        //Fabric.with(fabric);
        Log.v(TAG, "My activity started !!!");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }
}
