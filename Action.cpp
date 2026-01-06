#include "Action.h"
void AddAction::executeundo() {
	this->repo->removeMovie(this->movie);
}
void AddAction::executeredo() {
	this->repo->addMovie(this->movie);
}
void RemoveAction::executeundo() {
	this->repo->addMovie(this->movie);
}
void RemoveAction::executeredo() {
	this->repo->removeMovie(this->movie);
}


void UpdateAction::executeundo() {
	this->repo->updateMovie(this->pos,this->oldmovie);
}
void UpdateAction::executeredo() {
	this->repo->updateMovie(this->pos,this->movie);
}