-- Additional quiz data with randomized correct answers
-- Run this after the initial database_schema.sql

USE quiz_app;

-- Insert more quizzes
INSERT INTO quizzes (title, description, created_at) VALUES
('JavaScript Fundamentals', 'Test your knowledge of JavaScript basics and ES6 features', NOW()),
('Database Design', 'Questions about relational databases, SQL, and normalization', NOW()),
('Web Development', 'HTML, CSS, and web development best practices', NOW()),
('Python Programming', 'Python syntax, data structures, and object-oriented programming', NOW()),
('Computer Networks', 'Networking concepts, protocols, and security', NOW());

-- Get the quiz IDs (assuming they start from ID 2 since Java quiz is ID 1)
SET @js_quiz_id = 2;
SET @db_quiz_id = 3;
SET @web_quiz_id = 4;
SET @python_quiz_id = 5;
SET @network_quiz_id = 6;

-- JavaScript Fundamentals Quiz Questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES
(@js_quiz_id, 'What is the correct way to declare a variable in JavaScript ES6?', 1),
(@js_quiz_id, 'Which method is used to add an element to the end of an array?', 2),
(@js_quiz_id, 'What does the "this" keyword refer to in JavaScript?', 3),
(@js_quiz_id, 'Which of the following is NOT a JavaScript data type?', 4),
(@js_quiz_id, 'What is the output of: console.log(typeof null)?', 5),
(@js_quiz_id, 'Which method is used to convert a string to uppercase?', 6),
(@js_quiz_id, 'What is a closure in JavaScript?', 7),
(@js_quiz_id, 'Which operator is used for strict equality comparison?', 8);

-- JavaScript Answer Options (randomized correct positions)
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: What is the correct way to declare a variable in JavaScript ES6? (correct: let/const)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 1), 'var myVar;', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 1), 'let myVar;', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 1), 'variable myVar;', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 1), 'declare myVar;', 0, 4),

-- Question 2: Which method is used to add an element to the end of an array? (correct: push)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 2), 'append()', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 2), 'add()', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 2), 'insert()', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 2), 'push()', 1, 4),

-- Question 3: What does the "this" keyword refer to in JavaScript? (correct: context object)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 3), 'The current function', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 3), 'The global object', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 3), 'The context object that called the function', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 3), 'The previous function', 0, 4),

-- Question 4: Which of the following is NOT a JavaScript data type? (correct: character)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 4), 'character', 1, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 4), 'string', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 4), 'boolean', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 4), 'number', 0, 4),

-- Question 5: What is the output of: console.log(typeof null)? (correct: object)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 5), 'null', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 5), 'undefined', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 5), 'object', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 5), 'string', 0, 4),

-- Question 6: Which method is used to convert a string to uppercase? (correct: toUpperCase)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 6), 'upperCase()', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 6), 'toUpper()', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 6), 'toUpperCase()', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 6), 'upper()', 0, 4),

-- Question 7: What is a closure in JavaScript? (correct: function with access to outer scope)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 7), 'A function that returns another function', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 7), 'A function with access to variables in its outer scope', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 7), 'A private function', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 7), 'A function without parameters', 0, 4),

-- Question 8: Which operator is used for strict equality comparison? (correct: ===)
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 8), '==', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 8), '=', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 8), '===', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @js_quiz_id AND question_order = 8), '!=', 0, 4);

-- Database Design Quiz Questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES
(@db_quiz_id, 'What is the primary purpose of database normalization?', 1),
(@db_quiz_id, 'Which SQL command is used to retrieve data from a database?', 2),
(@db_quiz_id, 'What is a foreign key?', 3),
(@db_quiz_id, 'Which normal form eliminates partial dependencies?', 4),
(@db_quiz_id, 'What does ACID stand for in database transactions?', 5),
(@db_quiz_id, 'Which JOIN returns only matching records from both tables?', 6);

-- Database Answer Options (randomized correct positions)
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: What is the primary purpose of database normalization? (correct: eliminate redundancy)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 1), 'To increase query speed', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 1), 'To eliminate data redundancy and improve data integrity', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 1), 'To add more tables', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 1), 'To create indexes', 0, 4),

-- Question 2: Which SQL command is used to retrieve data from a database? (correct: SELECT)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 2), 'GET', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 2), 'FETCH', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 2), 'RETRIEVE', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 2), 'SELECT', 1, 4),

-- Question 3: What is a foreign key? (correct: reference to primary key)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 3), 'A field that references the primary key of another table', 1, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 3), 'A unique identifier for each record', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 3), 'An index on a table', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 3), 'A backup key', 0, 4),

-- Question 4: Which normal form eliminates partial dependencies? (correct: 2NF)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 4), '1NF', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 4), '3NF', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 4), '2NF', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 4), 'BCNF', 0, 4),

-- Question 5: What does ACID stand for in database transactions? (correct: Atomicity, Consistency, Isolation, Durability)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 5), 'Access, Control, Integrity, Data', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 5), 'Atomicity, Consistency, Isolation, Durability', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 5), 'Authentication, Certification, Implementation, Design', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 5), 'Application, Connection, Interface, Database', 0, 4),

-- Question 6: Which JOIN returns only matching records from both tables? (correct: INNER JOIN)
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 6), 'LEFT JOIN', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 6), 'RIGHT JOIN', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 6), 'FULL JOIN', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @db_quiz_id AND question_order = 6), 'INNER JOIN', 1, 4);

-- Web Development Quiz Questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES
(@web_quiz_id, 'Which HTML tag is used to define the structure of a web page?', 1),
(@web_quiz_id, 'What does CSS stand for?', 2),
(@web_quiz_id, 'Which CSS property is used to change text color?', 3),
(@web_quiz_id, 'What is the correct HTML tag for creating a hyperlink?', 4),
(@web_quiz_id, 'Which HTTP method is typically used to submit form data?', 5),
(@web_quiz_id, 'What is responsive web design?', 6);

-- Web Development Answer Options (randomized correct positions)
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: Which HTML tag is used to define the structure of a web page? (correct: html)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 1), '<body>', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 1), '<html>', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 1), '<head>', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 1), '<div>', 0, 4),

-- Question 2: What does CSS stand for? (correct: Cascading Style Sheets)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 2), 'Computer Style Sheets', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 2), 'Creative Style Sheets', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 2), 'Cascading Style Sheets', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 2), 'Colorful Style Sheets', 0, 4),

-- Question 3: Which CSS property is used to change text color? (correct: color)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 3), 'font-color', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 3), 'text-color', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 3), 'color', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 3), 'background-color', 0, 4),

-- Question 4: What is the correct HTML tag for creating a hyperlink? (correct: <a>)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 4), '<a>', 1, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 4), '<link>', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 4), '<href>', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 4), '<url>', 0, 4),

-- Question 5: Which HTTP method is typically used to submit form data? (correct: POST)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 5), 'GET', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 5), 'PUT', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 5), 'POST', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 5), 'DELETE', 0, 4),

-- Question 6: What is responsive web design? (correct: adapts to different screen sizes)
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 6), 'Design that responds to user clicks', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 6), 'Design that adapts to different screen sizes and devices', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 6), 'Design with animations', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @web_quiz_id AND question_order = 6), 'Design that loads quickly', 0, 4);

-- Python Programming Quiz Questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES
(@python_quiz_id, 'Which keyword is used to define a function in Python?', 1),
(@python_quiz_id, 'What is the correct way to create a list in Python?', 2),
(@python_quiz_id, 'Which method is used to add an item to a Python list?', 3),
(@python_quiz_id, 'What is the output of: print(type([]))?', 4),
(@python_quiz_id, 'Which operator is used for exponentiation in Python?', 5),
(@python_quiz_id, 'What is a Python dictionary?', 6);

-- Python Answer Options (randomized correct positions)
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: Which keyword is used to define a function in Python? (correct: def)
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 1), 'function', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 1), 'def', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 1), 'define', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 1), 'func', 0, 4),

-- Question 2: What is the correct way to create a list in Python? (correct: [])
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 2), '{}', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 2), '()', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 2), '[]', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 2), '<>', 0, 4),

-- Question 3: Which method is used to add an item to a Python list? (correct: append)
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 3), 'add()', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 3), 'insert()', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 3), 'append()', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 3), 'push()', 0, 4),

-- Question 4: What is the output of: print(type([]))? (correct: <class 'list'>)
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 4), '<class ''list''>', 1, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 4), '<class ''array''>', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 4), '<type ''list''>', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 4), 'list', 0, 4),

-- Question 5: Which operator is used for exponentiation in Python? (correct: **)
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 5), '^', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 5), '**', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 5), 'pow', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 5), '*', 0, 4),

-- Question 6: What is a Python dictionary? (correct: key-value pairs)
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 6), 'An ordered collection of items', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 6), 'A collection of key-value pairs', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 6), 'A list of functions', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @python_quiz_id AND question_order = 6), 'A type of loop', 0, 4);

-- Computer Networks Quiz Questions
INSERT INTO questions (quiz_id, question_text, question_order) VALUES
(@network_quiz_id, 'What does TCP stand for?', 1),
(@network_quiz_id, 'Which layer of the OSI model handles routing?', 2),
(@network_quiz_id, 'What is the default port number for HTTP?', 3),
(@network_quiz_id, 'Which protocol is used for secure web browsing?', 4),
(@network_quiz_id, 'What does DNS stand for?', 5),
(@network_quiz_id, 'Which device operates at the Physical layer of the OSI model?', 6);

-- Network Answer Options (randomized correct positions)
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: What does TCP stand for? (correct: Transmission Control Protocol)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 1), 'Transfer Control Protocol', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 1), 'Transmission Control Protocol', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 1), 'Transport Communication Protocol', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 1), 'Terminal Control Protocol', 0, 4),

-- Question 2: Which layer of the OSI model handles routing? (correct: Network Layer)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 2), 'Transport Layer', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 2), 'Data Link Layer', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 2), 'Network Layer', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 2), 'Session Layer', 0, 4),

-- Question 3: What is the default port number for HTTP? (correct: 80)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 3), '443', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 3), '21', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 3), '80', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 3), '25', 0, 4),

-- Question 4: Which protocol is used for secure web browsing? (correct: HTTPS)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 4), 'HTTPS', 1, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 4), 'FTP', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 4), 'SMTP', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 4), 'TELNET', 0, 4),

-- Question 5: What does DNS stand for? (correct: Domain Name System)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 5), 'Data Network Service', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 5), 'Domain Name System', 1, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 5), 'Dynamic Network Service', 0, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 5), 'Distributed Name Server', 0, 4),

-- Question 6: Which device operates at the Physical layer of the OSI model? (correct: Hub)
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 6), 'Router', 0, 1),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 6), 'Switch', 0, 2),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 6), 'Hub', 1, 3),
((SELECT id FROM questions WHERE quiz_id = @network_quiz_id AND question_order = 6), 'Gateway', 0, 4);

-- Update the existing Java quiz to also have randomized correct answers
-- Delete existing Java quiz answers and re-insert with randomized positions
DELETE FROM answer_options WHERE question_id IN (SELECT id FROM questions WHERE quiz_id = 1);

-- Java Quiz Questions - Updated with randomized correct answers
INSERT INTO answer_options (question_id, option_text, is_correct, option_order) VALUES
-- Question 1: What is the correct way to declare a string in Java? (correct: String str = "Hello";)
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 1), 'string str = "Hello";', 0, 1),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 1), 'String str = "Hello";', 1, 2),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 1), 'str = "Hello";', 0, 3),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 1), 'new String str = "Hello";', 0, 4),

-- Question 2: Which keyword is used to inherit a class in Java? (correct: extends)
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 2), 'implements', 0, 1),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 2), 'inherits', 0, 2),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 2), 'extends', 1, 3),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 2), 'derives', 0, 4),

-- Question 3: What is the size of int data type in Java? (correct: 4 bytes)
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 3), '2 bytes', 0, 1),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 3), '8 bytes', 0, 2),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 3), '4 bytes', 1, 3),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 3), '1 byte', 0, 4),

-- Question 4: Which method is called when an object is created in Java? (correct: constructor)
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 4), 'constructor', 1, 1),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 4), 'main()', 0, 2),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 4), 'init()', 0, 3),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 4), 'start()', 0, 4),

-- Question 5: Which of these is NOT a Java keyword? (correct: goto)
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 5), 'static', 0, 1),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 5), 'goto', 1, 2),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 5), 'final', 0, 3),
((SELECT id FROM questions WHERE quiz_id = 1 AND question_order = 5), 'abstract', 0, 4);

COMMIT; 