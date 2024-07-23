package com.example.cpuinfotest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Build;
import android.os.Bundle;
import android.widget.TextView;

import com.example.cpuinfotest.databinding.ActivityMainBinding;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'cpuinfotest' library on application startup.
    static {
        System.loadLibrary("cpuinfotest");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'cpuinfotest' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


}