select movies.title from movies, stars, people where movies.id = stars.movie_id and stars.person_id = people.id and people.name = 'Johnny Depp' and stars.movie_id in (select movie_id from stars, people where stars.person_id = people.id and people.name = 'Helena Bonham Carter');
