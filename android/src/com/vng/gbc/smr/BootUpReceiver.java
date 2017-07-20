package com.vng.gbc.smr;
import android.content.Intent;
import android.content.Context;
import android.content.BroadcastReceiver;

public class BootUpReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
                Intent i = new Intent(context, MyActivity.class);
                i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(i);
        }

}
