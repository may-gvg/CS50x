select distinct people.name from ratings, people, movies, directors where directors.movie_id = movies.id and people.id = directors.person_id and ratings.movie_id = movies.id and rating >= 9;
