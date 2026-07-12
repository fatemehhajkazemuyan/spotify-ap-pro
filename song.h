#ifndef SONG_H
#define SONG_H

#include <string>

using namespace std;

class Song {
private:
    int id;
    string name;
    int releaseYear;
    string genre;
    string filePath;
    int artistId;

public:
    Song(int _id, string _name, int _releaseYear, string _genre, string _filePath, int _artistId) {
        id = _id;
        name = _name;
        releaseYear = _releaseYear;
        genre = _genre;
        filePath = _filePath;
        artistId = _artistId;
    }

    int getId() { return id; }
    string getName() { return name; }
    int getReleaseYear() { return releaseYear; }
    string getGenre() { return genre; }
    string getFilePath() { return filePath; }
    int getArtistId() { return artistId; }
};

#endif // SONG_H
