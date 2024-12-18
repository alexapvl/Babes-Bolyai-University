USE A5;


CREATE TABLE Ta (
    aid INT PRIMARY KEY,
    a2 INT UNIQUE,
    a3 VARCHAR(10)
);

INSERT INTO Ta (aid, a2, a3) VALUES
(1, 100, 'a1'),
(2, 200, 'a2'),
(3, 300, 'a3');

CREATE TABLE Tb (
    bid INT PRIMARY KEY,
    b2 INT
);

INSERT INTO Tb (bid, b2) VALUES
(1, 10),
(2, 20),
(3, 30);

CREATE TABLE Tc (
    cid INT PRIMARY KEY,
    aid INT,
    bid INT,
    FOREIGN KEY (aid) REFERENCES Ta(aid),
    FOREIGN KEY (bid) REFERENCES Tb(bid)
);

INSERT INTO Tc (cid, aid, bid) VALUES
(1, 2, 3),
(2, 3, 1),
(3, 1, 2),
(4, 3, 2),
(5, 1, 3),
(6, 2, 1);

-- Obs: When not specified, the default is nonclustered

-- Clustered Index Scan on Ta
SELECT * FROM Ta;

-- Clustered Index Seek on Ta
SELECT * FROM Ta WHERE aid = 2;

-- Nonclustered Index Scan on Ta
SELECT a2 FROM Ta;

-- Nonclustered Index Seek on Ta
SELECT a2 FROM Ta WHERE a2 = 200;

-- Key Lookup on Ta because of the a3 column
SELECT a2, a3 FROM Ta WHERE a2 = 100;

-- without the non clustered index on Tb, the query would be a Clustered Index Scan
SELECT * FROM Tb WHERE b2 = 200;
-- DROP INDEX IX_Tb_b2 ON Tb;

-- create the nonclustered index on Tb
CREATE NONCLUSTERED INDEX IX_Tb_b2 ON Tb(b2);

-- Nonclustered Index Seek on Tb, should do an Index Seek instead, which is much faster for large tables
SELECT * FROM Tb WHERE b2 = 200;

-- create view on two tables
CREATE VIEW MyView 
AS
SELECT Ta.aid, Ta.a2, Tc.cid
FROM Ta
JOIN Tc ON Ta.aid = Tc.aid;

SELECT * FROM MyView WHERE a2 = 200;
-- DROP INDEX IX_Ta_aid_a2 ON Ta;

-- create nonclustered index on Ta to optimise queries which use both aid and a2
CREATE NONCLUSTERED INDEX IX_Ta_aid_a2 ON Ta(aid, a2); 

-- chech to see execution plan changes
SELECT * FROM MyView WHERE a2 = 200;
