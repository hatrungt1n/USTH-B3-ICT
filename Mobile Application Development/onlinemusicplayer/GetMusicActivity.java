package com.example.onlinemusicplayer;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import java.util.ArrayList;

public class GetMusicActivity extends AppCompatActivity {

    private static final int REQUEST_PERMISSION = 99;

    ArrayList<Song> songArrayList;
    ListView listSongs;
    SongsAdapter songsAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_get_music);

        listSongs = findViewById(R.id.listSongs);

        songArrayList = new ArrayList<>();

        songsAdapter = new SongsAdapter(this, R.layout.song, songArrayList);

        listSongs.setAdapter(songsAdapter);

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, REQUEST_PERMISSION);
            return;
        } else {
            getSongs();
        }

        listSongs.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Song song = songArrayList.get(position);
                Intent openMusicPlayer = new Intent(GetMusicActivity.this, MusicPlayerActivity.class);

                openMusicPlayer.putExtra("song", song);
                startActivity(openMusicPlayer);
            }
        });
    }


    @SuppressLint("MissingSuperCall")
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(requestCode == REQUEST_PERMISSION) {
            if(grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                getSongs();
            }
        }
    }

    private void getSongs() {
        ContentResolver contentResolver = getContentResolver();
        Uri songUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;

        Cursor songCursor = contentResolver.query(songUri, null, null, null, null);

        if(songCursor != null && songCursor.moveToFirst()) {

            int indexSongTitle = songCursor.getColumnIndex(MediaStore.Audio.Media.TITLE);
            int indexArtist = songCursor.getColumnIndex(MediaStore.Audio.Media.ARTIST);
            int indexData = songCursor.getColumnIndex(MediaStore.Audio.Media.DATA);

            do {
                String title = songCursor.getString(indexSongTitle);
                String artist = songCursor.getString(indexArtist);
                String path = songCursor.getString(indexData);
                songArrayList.add(new Song(title, artist, path));
            } while (songCursor.moveToNext());
        }
        songsAdapter.notifyDataSetChanged();
    }
}