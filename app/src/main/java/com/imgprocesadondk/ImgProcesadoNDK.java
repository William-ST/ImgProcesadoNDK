package com.imgprocesadondk;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;

public class ImgProcesadoNDK extends AppCompatActivity {

    private final static String TAG = ImgProcesadoNDK.class.getCanonicalName();
    private Bitmap currentBitmap = null;
    private Bitmap bitmapGrises = null;
    private Bitmap bitmapSepia = null;
    private Bitmap bitmapFrame = null;
    private Bitmap bitmapSobel = null;
    private ImageView ivDisplay = null;

    static final int REQUEST_TAKE_PHOTO = 1;
    private final int PERMISSION_REQUEST_CODE = 2;

    private File mPictureFile = null;
    private String PICTURE_FILE_NAME = "photoCamera.jpg";
    private int widthPreImgFace = 0;
    private int heightPreImgFace = 0;

    static {
        System.loadLibrary("imgprocesadondk");
    }

    public native void convertirGrises(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void convertirSepia(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void onAddFrameC(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void callbackJavaMethod(Bitmap bitmapIn, Bitmap bitmapOut);

    public native void convertirSobel(Bitmap bitmapIn, Bitmap bitmapOut);

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_TAKE_PHOTO && resultCode == RESULT_OK) {

            try {
                currentBitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), Uri.fromFile(mPictureFile));
            } catch (IOException e) {
                e.printStackTrace();
                Log.d(TAG, "error: " + e.toString());
            }

            setPictureFromFile(ivDisplay);
        }
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        Log.i(TAG, "Imagen antes de modificar");
        ivDisplay = (ImageView) findViewById(R.id.ivDisplay);
        BitmapFactory.Options options = new BitmapFactory.Options();
        // Asegurar que la imagen tiene 24 bits de color
        options.inPreferredConfig = Bitmap.Config.ARGB_8888;
        currentBitmap = BitmapFactory.decodeResource(this.getResources(), R.drawable.sampleimage, options);
        if (currentBitmap != null)
            ivDisplay.setImageBitmap(currentBitmap);

        ivDisplay.post(new Runnable() {
            @Override
            public void run() {
                widthPreImgFace = ivDisplay.getWidth();
                heightPreImgFace = ivDisplay.getHeight();
            }
        });

    }

    public void onResetImagen() {
        Log.i(TAG, "Resetear Imagen");
        ivDisplay.setImageBitmap(currentBitmap);
    }

    public void onConvertirGrises() {
        Log.i(TAG, "Conversion a escala de grises");
        bitmapGrises = Bitmap.createBitmap(currentBitmap.getWidth(),
                currentBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        convertirGrises(currentBitmap, bitmapGrises);
        ivDisplay.setImageBitmap(bitmapGrises);
    }

    public void onConvertirSepia() {
        Log.i(TAG, "Conversion a sepia");
        bitmapSepia = Bitmap.createBitmap(currentBitmap.getWidth(),
                currentBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        convertirSepia(currentBitmap, bitmapSepia);
        ivDisplay.setImageBitmap(bitmapSepia);
    }

    public void onAddFrameC() {
        Log.i(TAG, "Agregar marco c");
        bitmapFrame = Bitmap.createBitmap(currentBitmap.getWidth(),
                currentBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        onAddFrameC(currentBitmap, bitmapFrame);
        ivDisplay.setImageBitmap(bitmapFrame);
    }

    public void onAddFrameJava() {
        Log.i(TAG, "Agregar marco java");
        bitmapFrame = Bitmap.createBitmap(currentBitmap.getWidth(),
                currentBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        callbackJavaMethod(currentBitmap, bitmapFrame);
        ivDisplay.setImageBitmap(bitmapFrame);
    }

    public void onSobel() {
        Log.i(TAG, "Sobel");
        bitmapSobel = Bitmap.createBitmap(currentBitmap.getWidth(),
                currentBitmap.getHeight(), Bitmap.Config.ARGB_8888);
        convertirSobel(currentBitmap, bitmapSobel);
        ivDisplay.setImageBitmap(bitmapSobel);
    }

    private void requestPermissions() {
        requestPermissions(new String[]{Manifest.permission.CAMERA},
                PERMISSION_REQUEST_CODE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                dispatchTakePictureIntent();
            } else {
                showMessage();
            }
        }
    }

    public void takePhoto(View view) {
        if (checkPermissions())
            dispatchTakePictureIntent();
    }

    private void dispatchTakePictureIntent() {
        if (mPictureFile == null) {
            mPictureFile = new File(getFilesDir(), PICTURE_FILE_NAME);
        }
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            Uri photoURI = FileProvider.getUriForFile(this, BuildConfig.APPLICATION_ID + ".fileprovider", mPictureFile);
            takePictureIntent.putExtra(MediaStore.ACTION_IMAGE_CAPTURE, 1);
            takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
            startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
        }
    }

    private boolean checkPermissions() {
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED) {

            if (shouldShowRequestPermissionRationale(Manifest.permission.CAMERA)) {
                showMessage();
                requestPermissions();
            } else {
                requestPermissions();
            }
            return false;
        }
        return true;
    }

    private void setPictureFromFile(ImageView imageView) {
        BitmapFactory.Options bmOptions = new BitmapFactory.Options();
        bmOptions.inJustDecodeBounds = true;

        int photoW = bmOptions.outWidth;
        int photoH = bmOptions.outHeight;

        int scaleFactor = 0;
        if (widthPreImgFace > 0 && heightPreImgFace > 0) {
            scaleFactor = Math.min(photoW / widthPreImgFace, photoH / heightPreImgFace);
        } else {
            scaleFactor = Math.min(photoW / 150, photoH / 150);
        }

        bmOptions.inJustDecodeBounds = false;
        bmOptions.inSampleSize = scaleFactor;
        bmOptions.inPurgeable = true;

        Bitmap bitmap = BitmapFactory.decodeFile(mPictureFile.getAbsolutePath(), bmOptions);
        imageView.setImageBitmap(bitmap);

    }

    private void showMessage() {
        Toast.makeText(this, "Debes aceptar el permiso para utilizar la cámara", Toast.LENGTH_SHORT).show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_original:
                onResetImagen();
                break;
            case R.id.menu_gray:
                onConvertirGrises();
                break;
            case R.id.menu_sepia:
                onConvertirSepia();
                break;
            case R.id.menu_frame_c:
                onAddFrameC();
                break;
            case R.id.menu_frame_java:
                onAddFrameJava();
                break;
            case R.id.menu_sobel:
                onSobel();
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    public static boolean hayPixel(int x, int y) {
        Log.d(TAG, "call java hayPixel x:y: "+x+":"+y);
        return x%10 == y%10;
    }
}