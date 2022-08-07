select distinct name from stars, people where stars.person_id = people.id and movie_id in
(select movie_id from stars, people where stars.person_id = people.id and name = "Kevin Bacon" and birth = 1958 ) and name is not "Kevin Bacon";



