#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include <vector>

using namespace std;

class Album {
private:
    int id;
    string name;
    int artistId;
    int releaseYear;
    vector<int> songIds;

public:
    // سازنده کلاس
    Album(int _id, string _name, int _artistId, int _releaseYear) {
        id = _id;
        name = _name;
        artistId = _artistId;
        releaseYear = _releaseYear;
    }

    // توابع گتر ساده و const
    int getId() const { return id; }
    string getName() const { return name; }
    int getArtistId() const { return artistId; }
    int getReleaseYear() const { return releaseYear; }
    vector<int> getSongIds() const { return songIds; }

    // توابع ستر و کمکی ویرایش آلبوم
    void setName(string newName) { name = newName; }

    // افزودن آهنگ به آلبوم
    void addSong(int songId) {
        songIds.push_back(songId);
    }

    // حذف آهنگ از آلبوم (برای بخش ویرایش آلبوم توسط هنرمند)
    void removeSong(int songId) {
        for (size_t i = 0; i < songIds.size(); i++) {
            if (songIds[i] == songId) {
                songIds.erase(songIds.begin() + i);
                break; // حذف شد، از حلقه خارج شو
            }
        }
    }
};

#endif // ALBUM_H
