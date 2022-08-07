select count(*) from movies, ratings where movies.id = ratings.movie_id and rating = 10;
