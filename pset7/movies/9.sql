select distinct people.name from people, movies, stars where stars.movie_id = movies.id and people.id = stars.person_id and movies.year = 2004 order by birth;
