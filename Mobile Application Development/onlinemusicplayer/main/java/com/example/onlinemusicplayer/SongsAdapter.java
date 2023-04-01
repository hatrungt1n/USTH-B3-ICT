package com.example.onlinemusicplayer;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.List;

public class SongsAdapter extends ArrayAdapter<Song> {

    public SongsAdapter(@NonNull Context context, int resoure, @NonNull List<Song> objects) {
        super(context, 0, objects);
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        convertView = LayoutInflater.from(parent.getContext()).inflate(R.layout.song, null);

        TextView songTitle = convertView.findViewById(R.id.songTitle);
        TextView artist = convertView.findViewById(R.id.artist);

        Song song = getItem(position);
        songTitle.setText(song.getTitle());
        artist.setText(song.getArtist());

        return convertView;
    }
}
