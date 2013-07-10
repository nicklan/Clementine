#ifndef OUTGOINGDATACREATOR_H
#define OUTGOINGDATACREATOR_H

#include <QTcpSocket>
#include <QImage>
#include <QList>
#include <QTimer>

#include "core/player.h"
#include "core/application.h"
#include "engines/enginebase.h"
#include "engines/engine_fwd.h"
#include "playlist/playlist.h"
#include "playlist/playlistmanager.h"
#include "playlist/playlistbackend.h"
#include "songinfo/collapsibleinfopane.h"
#include "songinfo/songinfofetcher.h"
#include "songinfo/songinfoprovider.h"
#include "songinfo/songinfotextview.h"
#include "songinfo/ultimatelyricsprovider.h"
#include "songinfo/ultimatelyricsreader.h"
#include "remotecontrolmessages.pb.h"
#include "remoteclient.h"
#include <boost/scoped_ptr.hpp>

typedef QList<SongInfoProvider*> ProviderList;

class OutgoingDataCreator : public QObject {
    Q_OBJECT
public:
  OutgoingDataCreator(Application* app);
  ~OutgoingDataCreator();

  void SetClients(QList<RemoteClient*>* clients);

public slots:
  void SendClementineInfo();
  void SendAllPlaylists();
  void SendAllActivePlaylists();
  void SendFirstData(bool send_playlist_songs);
  void SendPlaylistSongs(int id);
  void PlaylistChanged(Playlist*);
  void VolumeChanged(int volume);
  void PlaylistAdded(int id, const QString& name, bool favorite);
  void PlaylistDeleted(int id);
  void PlaylistClosed(int id);
  void PlaylistRenamed(int id, const QString& new_name);
  void ActiveChanged(Playlist*);
  void CurrentSongChanged(const Song& song, const QString& uri, const QImage& img);
  void StateChanged(Engine::State);
  void SendKeepAlive();
  void SendRepeatMode(PlaylistSequence::RepeatMode mode);
  void SendShuffleMode(PlaylistSequence::ShuffleMode mode);
  void UpdateTrackPosition();
  void DisconnectAllClients();
  void GetLyrics();
  void SendLyrics(int id, const SongInfoFetcher::Result& result);

private:
  Application* app_;
  QList<RemoteClient*>* clients_;
  Song current_song_;
  QString current_uri_;
  QImage current_image_;
  Engine::State last_state_;
  QTimer* keep_alive_timer_;
  QTimer* track_position_timer_;
  int keep_alive_timeout_;

  boost::scoped_ptr<UltimateLyricsReader> ultimate_reader_;
  ProviderList provider_list_;
  QMap<int, SongInfoFetcher::Result> results_;
  SongInfoFetcher* fetcher_;

  void SendDataToClients(pb::remote::Message* msg);
  void SetEngineState(pb::remote::ResponseClementineInfo* msg);
  void CreateSong(
      const Song& song,
      const QImage& art,
      const int index,
      pb::remote::SongMetadata* song_metadata);
};

#endif // OUTGOINGDATACREATOR_H
