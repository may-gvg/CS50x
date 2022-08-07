-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT description from crime_scene_reports where street = "Chamberlin Street" and month = 7 and day = 28 ;


SELECT name, phone_number, passport_number from courthouse_security_logs
join people on courthouse_security_logs.license_plate = people.license_plate and day = 28 and hour = 10 and minute <=30
and activity = "exit"

-- out 1: Patrick, Ernest, Amber, Danielle, Roger, Elizabeth, Russell, Evelyn

SELECT name from atm_transactions
join people on people.id = bank_accounts.person_id
join bank_accounts ON bank_accounts.account_number = atm_transactions.account_number
where day = 28 and month = 7 and atm_location = "Fifer Street"

-- out 2 Ernest, Robert, RussellRoyBobby, Elizabeth, Danielle, Madison, Victoria

-- out 1 Patrick, Ernest, Amber, Danielle, Roger, Elizabeth, Russell, Evelyn

--out 3  Ernest, Russell,  Elizabeth, Danielle,

SELECT name from people join passengers on passengers.passport_number = people.passport_number
where flight_id = (select id from flights where month = 7 and day = 29 and year = 2020 order by hour, minute limit 1 )

-- out 4 Doris, Roger, Ernest, Edward, Evelyn, Madison, Bobby, Danielle

-- out 5 Ernest, Russell,  Elizabeth, Danielle,

-- out6  Ernest, Danielle

SELECT DISTINCT name from people JOIN phone_calls on people.phone_number = phone_calls.caller where
duration < 60 and day = 28 and month = 7 and year = 2020;

-- out 7  Roger, Evelyn, Ernest, Madison, Russell, Kimberly, Bobby, Victoria

-- out 6  Ernest, Danielle,

-- Ti --> Ernest


SELECT name from phone_calls JOIN people on phone_calls.receiver = people.phone_number where
duration < 60 and day = 28 and caller = "(367) 555-5533"

-- checking Ernest phone no, ->  Berthold

SELECT full_name from flights join airports on destination_airport_id = airports.id
where flights.origin_airport_id = 8  and day = 29  and month = 7 order by hour limit 1;

-- Heathrow Airport

SELECT city FROM airports where full_name = 'Heathrow Airport'

-- London

