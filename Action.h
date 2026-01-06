#pragma once
#include "Movie.h"
#include "Repository.h"

class Action {
public:
	Action(){}
	virtual void executeundo() = 0;
	virtual void executeredo() = 0;

	virtual ~Action(){}
};

class AddAction :public Action {
private:
	Repository* repo;
	Movie movie;
public:
	AddAction(Repository* repo, Movie movie): repo{repo}, movie{movie}{}
	~AddAction(){}
	void executeundo() override;
	void executeredo() override;

};
class RemoveAction : public Action {
private:
	Repository* repo;
	Movie movie;
public:
	RemoveAction(Repository* repo, Movie movie) : repo{ repo }, movie{ movie } {}
	~RemoveAction() {}
	void executeundo() override;
	void executeredo() override;

};


class UpdateAction : public Action {
private:
	Repository* repo;
	Movie movie, oldmovie;
	int pos;
public:
	UpdateAction(Repository* repo, Movie movie, Movie oldmovie, int pos) : repo{ repo }, movie{ movie }, oldmovie{ oldmovie }, pos { pos } {}
	~UpdateAction() {}
	void executeundo() override;
	void executeredo() override;

};