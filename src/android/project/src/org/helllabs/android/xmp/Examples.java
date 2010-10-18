package org.helllabs.android.xmp;

import java.io.File;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.FileOutputStream;


public class Examples {
	AssetManager am;
	String[] assets;
	Context context;
	
	public Examples(Context c) {
		context = c;
	}
	
	public int install(String path) {
		File dir = new File(path);
		
		Log.v("xmp", "path = " + path);
		
		/*if (dir.isDirectory())
			return 0;
		
		dir.mkdirs();*/
		
		am = context.getResources().getAssets();
		
		try {
			assets = am.list("mod");
			
			if (assets == null)
				return 0;
		
			for (int i = 0; i < assets.length; i++) {
				Log.v("xmp", "asset = " + assets[i]);
				copyAsset(am.open("mod/" + assets[i]), path + "/" + assets[i]);
			}
		} catch (IOException e) {
			return -1;
		}
		
		return 0;
	}

	
	private int copyAsset(InputStream in, String dst) {
		byte[] buf = new byte[1024];
		int len;
		
		Log.v("xmp", "dst = " + dst);
		
		try{			
		      OutputStream out = new FileOutputStream(new File(dst));
		      while ((len = in.read(buf)) > 0) {
		    	  out.write(buf, 0, len);
		      }
		      in.close();
		      out.close();
		} catch (FileNotFoundException e) {
			return -1;
		} catch (IOException e) {
			return -1;
		}
		
		return 0;
	}
}
