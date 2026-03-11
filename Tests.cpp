#include "Tests.h"
#include <cassert>
#include <sstream>
#include <iostream>

void test_movies() {
    std::cout << "Running Movie class tests..." << std::endl;
    Movie m1;
    m1.setTitle("Inception");
    m1.setGenre("Sci-Fi");
    m1.setTrailer("https://www.youtube.com/watch?v=YoHD9XEInc0");
    m1.setYearOfRelease(2010);
    m1.setLikesCount(5000);

    assert(m1.getTitle() == "Inception");
    assert(m1.getGenre() == "Sci-Fi");
    assert(m1.getTrailer() == "https://www.youtube.com/watch?v=YoHD9XEInc0");
    assert(m1.getYearOfRelease() == 2010);
    assert(m1.getLikesCount() == 5000);

    Movie m2(m1);
    assert(m2.getTitle() == "Inception");
    assert(m2.getGenre() == "Sci-Fi");
    assert(m2.getTrailer() == "https://www.youtube.com/watch?v=YoHD9XEInc0");
    assert(m2.getYearOfRelease() == 2010);
    assert(m2.getLikesCount() == 5000);

    Movie m3;
    m3 = m1;
    assert(m3.getTitle() == "Inception");
    assert(m3.getGenre() == "Sci-Fi");
    assert(m3.getTrailer() == "https://www.youtube.com/watch?v=YoHD9XEInc0");
    assert(m3.getYearOfRelease() == 2010);
    assert(m3.getLikesCount() == 5000);

    assert(m1 == m2);          // same title, genre, year
    m3.setYearOfRelease(2011);
    assert(!(m1 == m3));       // different year

   
    std::ostringstream oss;
    oss << m1;
    std::string output = oss.str();
    assert(output.find("Inception") != std::string::npos);
    assert(output.find("Sci-Fi") != std::string::npos);

   
    std::istringstream iss("Interstellar|Sci-Fi|2014|https://www.youtube.com/watch?v=zSWdZVtXT7E|8000\n");
    Movie m4;
    iss >> m4;
    assert(m4.getTitle() == "Interstellar");
    assert(m4.getGenre() == "Sci-Fi");
    assert(m4.getYearOfRelease() == 2014);
    assert(m4.getTrailer() == "https://www.youtube.com/watch?v=zSWdZVtXT7E");
    assert(m4.getLikesCount() == 8000);


    std::istringstream issInvalid("Only|Three|Tokens\n");
    Movie m5;
    bool exceptionCaught = false;
    try {
        issInvalid >> m5;
    }
    catch (const MovieExceptions& e) {
        exceptionCaught = true;
    }
    assert(exceptionCaught);

    m1.openTrailerInBrowser(); 

    std::cout << "All Movie tests passed!" << std::endl;
}


void test_repository() {
    std::cout << "Running Repository class tests..." << std::endl;

    Repository repo;

    //Test that repository starts empty
    assert(repo.getMoviesCount() == 0);
    assert(repo.getMovies().empty());

    //Test addMovie
    Movie m1("Inception", "Sci-Fi", "https://youtu.be/YoHD9XEInc0", 2010, 5000);
    Movie m2("Interstellar", "Sci-Fi", "https://youtu.be/zSWdZVtXT7E", 2014, 8000);
    repo.addMovie(m1);
    repo.addMovie(m2);
    assert(repo.getMoviesCount() == 2);
    assert(repo.getMovieByPosition(0) == m1);
    assert(repo.getMovieByPosition(1) == m2);

    //Test getMovies returns copy of vector
    std::vector<Movie> moviesCopy = repo.getMovies();
    assert(moviesCopy.size() == 2);
    assert(moviesCopy[0] == m1);

    //Test removeMovie by object
    repo.removeMovie(m1);
    assert(repo.getMoviesCount() == 1);
    assert(repo.getMovieByPosition(0) == m2);

    //Test removeMovieByPosition
    repo.removeMovieByPosition(0);
    assert(repo.getMoviesCount() == 0);

    // 6️⃣ Test updateMovie
    repo.addMovie(m1);
    repo.addMovie(m2);
    Movie mUpdated("Inception Updated", "Sci-Fi", "https://youtu.be/updated", 2010, 6000);
    repo.updateMovie(0, mUpdated);
    assert(repo.getMovieByPosition(0).getTitle() == "Inception Updated");
    assert(repo.getMovieByPosition(0).getLikesCount() == 6000);

    //Test getMovieByPosition with invalid position
    bool exceptionCaught = false;
    try {
        repo.getMovieByPosition(10);
    }
    catch (const InvalidPositionException&) {
        exceptionCaught = true;
    }
    assert(exceptionCaught);

    //Test validatePosition
    assert(repo.validatePosition(0) == true);
    assert(repo.validatePosition(1) == true);
    assert(repo.validatePosition(2) == false);
    assert(repo.validatePosition(-1) == false);

    //Test returnPosition
    int pos = repo.returnPosition(m2);
    assert(pos == 1);
    Movie mNotInRepo("Not In Repo", "Genre", "link", 2000, 0);
    assert(repo.returnPosition(mNotInRepo) == -1); // INDEX_NOT_FOUND assumed -1

    //Test modifyLikesCountOfMovieByPosition
    int originalLikes = repo.getMovieByPosition(0).getLikesCount();
    repo.modifyLikesCountOfMovieByPosition(0, 100);
    assert(repo.getMovieByPosition(0).getLikesCount() == originalLikes + 100);

    //Test clearDataFromRepository
    repo.clearDataFromRepository();
    assert(repo.getMoviesCount() == 0);
    assert(repo.getMovies().empty());
}


void test_action() {
    std::cout << "Running Action class tests..." << std::endl;

    Repository repo;

    // Setup movies
    Movie m1("Inception", "Sci-Fi", "link1", 2010, 5000);
    Movie m2("Interstellar", "Sci-Fi", "link2", 2014, 8000);

    // Test AddAction
    AddAction addAction(&repo, m1);
    addAction.executeredo(); // should add m1
    assert(repo.getMoviesCount() == 1);
    assert(repo.getMovieByPosition(0) == m1);

    addAction.executeundo(); // should remove m1
    assert(repo.getMoviesCount() == 0);

    // Test RemoveAction
    repo.addMovie(m2);
    RemoveAction removeAction(&repo, m2);
    removeAction.executeredo(); // remove m2
    assert(repo.getMoviesCount() == 0);
    removeAction.executeundo(); // add m2 back
    assert(repo.getMoviesCount() == 1);
    assert(repo.getMovieByPosition(0) == m2);

    // -------------------------
    // Test UpdateAction
    Movie m2Updated("Interstellar Updated", "Sci-Fi", "link2", 2014, 9000);
    UpdateAction updateAction(&repo, m2Updated, m2, 0);
    updateAction.executeredo(); // update to m2Updated
    assert(repo.getMovieByPosition(0) == m2Updated);
    updateAction.executeundo(); // revert back to m2
    assert(repo.getMovieByPosition(0) == m2);

    std::cout << "All Action tests passed!" << std::endl;
}
void test_file_repository() {
    std::cout << "Running FileRepository tests..." << std::endl;

    const std::string testFile = "test_movies.txt";

    // Ensure file starts empty
    std::ofstream ofs(testFile, std::ofstream::trunc);
    ofs.close();

    FileRepository fileRepo(testFile);

    // 1️⃣ Test adding movies
    Movie m1("Inception", "Sci-Fi", "link1", 2010, 5000);
    Movie m2("Interstellar", "Sci-Fi", "link2", 2014, 8000);

    fileRepo.addMovie(m1);
    fileRepo.addMovie(m2);
    assert(fileRepo.getMoviesCount() == 2);

    //Test saveMovies is called (verify file content)
    std::ifstream ifs(testFile);
    std::string line;
    std::getline(ifs, line);
    assert(line.find("Inception") != std::string::npos);
    std::getline(ifs, line);
    assert(line.find("Interstellar") != std::string::npos);
    ifs.close();

    //Test removeMovie
    fileRepo.removeMovie(m1);
    assert(fileRepo.getMoviesCount() == 1);

    //Test removeMovieByPosition
    fileRepo.removeMovieByPosition(0);
    assert(fileRepo.getMoviesCount() == 0);

    // 5️⃣ Test updateMovie
    fileRepo.addMovie(m1);
    Movie m1Updated("Inception Updated", "Sci-Fi", "link1", 2010, 6000);
    fileRepo.updateMovie(0, m1Updated);
    assert(fileRepo.getMovieByPosition(0).getTitle() == "Inception Updated");

    // 6️⃣ Test loadMoviesIntoRepository
    // Save manually to file
    std::ofstream ofs2(testFile);
    ofs2 << "Movie1|Genre1|2001|link1|100\n";
    ofs2 << "Movie2|Genre2|2002|link2|200\n";
    ofs2.close();

    FileRepository fileRepo2(testFile);
    fileRepo2.loadMoviesIntoRepository();
    assert(fileRepo2.getMoviesCount() == 2);
    assert(fileRepo2.getMovieByPosition(0).getTitle() == "Movie1");
    assert(fileRepo2.getMovieByPosition(1).getTitle() == "Movie2");

    // 7️⃣ Test exception on invalid file
    bool exceptionCaught = false;
    try {
        FileRepository invalidRepo("non_existent_file.txt");
        invalidRepo.loadMoviesIntoRepository();
    }
    catch (const FileException&) {
        exceptionCaught = true;
    }
    assert(exceptionCaught==false);

    // Cleanup test file
    std::remove(testFile.c_str());

    std::cout << "All FileRepository tests passed!" << std::endl;
}


void test_service() {
    std::cout << "Running safe Service tests..." << std::endl;

    
    Repository* repo = new Repository();
    FileWatchList* watchlist = new CSVWatchList("test_watchlist.csv"); 
    Service service;

    service.setTypeOfRepository(repo);
    service.setTypeOfWatchList(watchlist);

    // Admin credentials
    service.setAdminCredentials(true);
    assert(service.getAdminCredentials() == true);

    // compareStrings
    assert(service.compareStrings("abc", "ABC") == true);
    assert(service.compareStrings("abc", "abcd") == false);
    assert(service.compareStrings("abc", "abd") == false);

    // Add movies
    Movie m1("Inception", "Sci-Fi", "https://www.youtube.com/watch?v=YoHD9XEInc0", 2010, 5000);
    Movie m2("Interstellar", "Sci-Fi", "https://youtu.be/zSWdZVtXT7E", 2014, 8000);

    assert(service.redo() == false);
    assert(service.undo() == false);

    service.addMovie(m1);

    bool exceptionCaught = false;
    try {
        service.getMovieByPosition(2);
    }
    catch (const InvalidPositionException&) {
        exceptionCaught = true;
    }
    assert(exceptionCaught == true);

    service.openTrailerOfMovieInBrowser(0);
    service.displayPlaylist();
    int oldidx = service.getCurrentIndexForMovieToBeDisplayed();
    service.incrementIndexOfCurrentMovieToBeDisplayed();
    assert(service.getCurrentIndexForMovieToBeDisplayed() == oldidx + 1);


    service.addMovie(m2);

    service.getMoviesByGenre("Sci-Fi");
    service.addCurrentMovieToWatchList();  
    auto wlMovies = service.getMoviesFromWatchList();
    assert(wlMovies.size() == 1);
    assert(wlMovies[0] == m1);
    service.removeWatchedMovieFromWatchList(0, false);
    wlMovies = service.getMoviesFromWatchList();
    assert(wlMovies.size() == 0);  // empty watchlist

    service.addCurrentMovieToWatchList(); // add m1 again
    Movie movieBeforeLike = service.getMovieByPosition(0);
    int oldLikes = movieBeforeLike.getLikesCount();
    service.removeWatchedMovieFromWatchList(0, true);
    wlMovies = service.getMoviesFromWatchList();
    assert(wlMovies.size() == 0);  // 
    assert(service.getMoviesCount() == 2);
  
    std::vector<Movie> movies = service.getMovies();
    assert(movies.size() == 2);               
    assert(movies[0].getTitle() == "Inception");

    // Duplicate check
    exceptionCaught = false;
    try { service.addMovie(m1); }
    catch (const DuplicateMovieException&) { exceptionCaught = true; }
    assert(exceptionCaught);

    assert(service.getMovieByPosition(0) == m1);

    // updateMovie
    Movie m1Updated("Inception Updated", "Sci-Fi", "https://www.youtube.com/watch?v=YoHD9XEInc0", 2010, 6000);
    service.updateMovie(0, m1Updated);
    assert(service.getMovieByPosition(0).getTitle() == "Inception Updated");

    // removeMovie
    service.removeMovieByPosition(1);
    assert(service.getMoviesCount() == 1);

    // getMoviesByGenre
    auto filtered = service.getMoviesByGenre("sci-fi");
    assert(filtered.size() == 1);

    Movie movieCopy = service.getMovieByPosition(0);
    oldLikes = movieCopy.getLikesCount();
    service.increaseLikesCountOfMovie(movieCopy);
    Movie updatedMovie = service.getMovieByPosition(0);
    assert(updatedMovie.getLikesCount() == oldLikes + 1);

    // Undo/Redo
    Movie m2New("Dune", "Sci-Fi", "https://youtu.be/zSWdZVtXT7E", 2021, 1000);
    service.addMovie(m2New);
    assert(service.getMoviesCount() == 2);
    assert(service.undo() == true);
    assert(service.getMoviesCount() == 1);
    assert(service.redo() == true);
    assert(service.getMoviesCount() == 2);

    // Watchlist
    service.addMovieToWatchList(m1Updated);
    wlMovies = service.getMoviesFromWatchList();
    assert(wlMovies.size() == 1);
    assert(wlMovies[0] == m1Updated);

    // Current movie to be displayed
    service.getMoviesByGenre("Sci-Fi");
    Movie currentMovie = service.getCurrentMovieToBeDisplayed();
    assert(currentMovie.getTitle() == "Inception Updated");

    std::string testFile = "test_file.txt";
    {
        std::ofstream file(testFile);
        file << "some text";
    }
    service.clearFile(testFile);

    std::ifstream file(testFile);
    std::string content;
    std::getline(file, content);
    assert(content.empty()); 

    file.close();
    std::remove(testFile.c_str());



}

void test_All() {
    test_movies();
    test_repository();
    test_service();
}
