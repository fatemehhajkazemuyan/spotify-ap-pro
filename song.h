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
    int albumId;

public:

    Song(int _id, string _name, int _releaseYear, string _genre, string _filePath, int _artistId, int _albumId = 0) {
        id = _id;
        name = _name;
        releaseYear = _releaseYear;
        genre = _genre;
        filePath = _filePath;
        artistId = _artistId;
        albumId = _albumId;
    }


    int getId() const { return id; }
    string getName() const { return name; }
    int getReleaseYear() const { return releaseYear; }
    string getGenre() const { return genre; }
    string getFilePath() const { return filePath; }
    int getArtistId() const { return artistId; }
    int getAlbumId() const { return albumId; }

    void setName(string newName) { name = newName; }
    void setGenre(string newGenre) { genre = newGenre; }
    void setAlbumId(int newAlbumId) { albumId = newAlbumId; }
    void setReleaseYear(int newYear) { releaseYear = newYear; }
};

#endif // SONG_H
