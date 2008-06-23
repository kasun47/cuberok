#include "playlistcontainer.h"

/************************
 * 
 *    PlaylistContainer
 * 
 ************************/

PlaylistContainer::PlaylistContainer(QWidget *parent) 
 : QWidget(parent), curlist(0), actlist(0), counter(0),
 alv(true), arv(true), cov(true), trv(true), tiv(true), yev(true), gev(true), fiv(true), lev(true)
{
	vboxLayout = new QVBoxLayout(this);
	tabs = new QTabWidget(this);
	vboxLayout->addWidget(tabs);
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	connect(this, SIGNAL(internalnext()), this, SLOT(next()), Qt::QueuedConnection);
	int count = 0;
	QDir dir(QDir::homePath() + "/.cuberok/");
	QStringList filters;
	filters << "*.plist";
	dir.setNameFilters(filters);
	foreach(QString file, dir.entryList()) {
		int st = file.lastIndexOf('\\')+1;
		QString plname = file.mid(st, file.lastIndexOf('.')-st);
		newList(plname);
		count ++;
	}
	if(!count) addList();
}

PlaylistContainer::~PlaylistContainer()
{
	while(lists.count() > 0) {
		delete lists.last();
		lists.pop_back();
	}
	delete tabs;
	delete vboxLayout;
}

void PlaylistContainer::listStarted(PlaylistView* pl)
{
	foreach(PlaylistView *p, lists) {
		if(p == pl) {
			if(!actlist) {
				actlist = p;
			} else
				actlist = p;
		}
		else p->stop();
	}
}

void PlaylistContainer::addList()
{
	newList();
}

void PlaylistContainer::newList(QString listname)
{
	if(!listname.size()) listname = "Playlist "+QString::number(++counter);
	PlaylistView *pl = new PlaylistView(listname, this); 
	lists.append(pl);
	tabs->addTab(lists.last(), listname);
	curlist = lists.last();
	tabs->setCurrentIndex(tabs->count()-1);
	//curlist->setContextMenuPolicy(Qt::ActionsContextMenu);
	curlist->setAcceptDrops(true);
	curlist->setDragEnabled(true);
	curlist->setDragDropMode(QAbstractItemView::DragDrop);
	curlist->setDropIndicatorShown(true);
	curlist->setSortingEnabled(true);
	curlist->viewAlbum(alv);
	curlist->viewArtist(arv);
	curlist->viewComment(cov);
	curlist->viewTrack(trv);
	curlist->viewTitle(tiv);
	curlist->viewYear(yev);
	curlist->viewGenre(gev);
	curlist->viewFile(fiv);
	curlist->viewLength(lev);
	connect(pl, SIGNAL(status(QString)), this, SIGNAL(status(QString)));
	connect(pl, SIGNAL(songPosition(int)), this, SIGNAL(songPosition(int)));
	connect(pl, SIGNAL(started(PlaylistView*)), this, SLOT(listStarted(PlaylistView*)));
}

void PlaylistContainer::delList()
{
	if(curlist) {
		if(actlist == curlist) {
			actlist->stop();
			actlist = 0;
		}
		int i;
		for(i=0; i<lists.count(); i++)
			if(lists[i] == curlist) {
				curlist->disconnect();
				lists.removeAt(i);
				//tabs->removeTab(i);
				delete curlist;
				break;
			}
		if(i<tabs->count()) {
			tabs->setCurrentIndex(i);
			curlist = lists[i];
		} else if(tabs->count() > 0) {
			tabs->setCurrentIndex(tabs->count()-1);
			curlist = lists[tabs->count()-1];
		} else {
			curlist = 0;
		}
	}
}
void PlaylistContainer::renameList()
{
	// TODO
}
void PlaylistContainer::prev()
{ if(actlist) actlist->prev(); }
void PlaylistContainer::next()
{ if(actlist) actlist->next(); }
void PlaylistContainer::play()
{ 
	if(curlist) {
		actlist = curlist;
		actlist->play(); 
	}
}
void PlaylistContainer::pause(bool b)
{ 
    Player::Self().setPause(b);
}
void PlaylistContainer::repeat(bool mode)
{ 
    Player::Self().repeat_mode = mode;
}
void PlaylistContainer::shuffle(bool mode)
{ 
    Player::Self().shuffle_mode = mode;
}
void PlaylistContainer::setVolume(int volume)
{
    Player::Self().setVolume(volume);
}
void PlaylistContainer::clear()
{ if(curlist) curlist->clear(); }
void PlaylistContainer::queueNext()
{ if(curlist) curlist->queueNext(); }
void PlaylistContainer::viewAlbum(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewAlbum(b); 
  alv = b; }
void PlaylistContainer::viewArtist(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewArtist(b); 
  arv = b; }
void PlaylistContainer::viewComment(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewComment(b); 
  cov = b; }
void PlaylistContainer::viewTrack(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewTrack(b); 
  trv = b; }
void PlaylistContainer::viewTitle(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewTitle(b); 
  tiv = b; }
void PlaylistContainer::viewYear(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewYear(b); 
  yev = b; }
void PlaylistContainer::viewGenre(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewGenre(b); 
  gev = b; }
void PlaylistContainer::viewFile(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewFile(b); 
  fiv = b; }
void PlaylistContainer::viewLength(bool b)
{ foreach(PlaylistView *pl, lists) pl->viewLength(b); 
  lev = b; }
void PlaylistContainer::editTag()
{ if(curlist) curlist->editTag(); }

void PlaylistContainer::removeSong()
{ if(curlist) curlist->removeSong(); }

void PlaylistContainer::tabChanged(int i)
{
	if(i < lists.count()) {
		curlist = lists[i];
	}
}
