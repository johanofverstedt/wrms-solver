
Water Retention on Magic Squares Solver v0.11a

Project Description:
*******************************************************************************************
A Constraint-Based Local Search solver for the Water Retention on Magic Squares-problem.

The problem is a very hard combinatorial optimisation problem, invented by Craig Knecht.
More can be read about it at:

http://en.wikipedia.org/wiki/Water_retention_on_mathematical_surfaces

Craigs website for the problem:

http://www.knechtmagicsquare.paulscomputing.com/

The solver is mainly based on the theory and ideas from my bachelor thesis,
which I wrote at Uppsala University, Sweden, for the Astra Group which does
research about Constraint Programming and related technologies:

Water Retention on Magic Squares with Constraint-Based Local Search
http://urn.kb.se/resolve?urn=urn:nbn:se:uu:diva-176018

I 'm posting two solvers in this project. One written in C++ which is the fastest,
at least for the heavier objective functions, and a Dynadec Comet solver which lets you
easily post new constraints along with the Magic-constraints.
*******************************************************************************************

Release Description
*******************************************************************************************
This release contains the source-code and binaries for Windows and MacOSX and projects
for XCode 3.6 and Visual Studio 10.
*******************************************************************************************

Changes:
*******************************************************************************************
v0.11a

- Added support for the Associative Magic Square-constraints so the Associative Magic Squares
can be explored with this solver as well.
- Time-scale changed from milliseconds to seconds.
- Added average time calculation which gives the average time of the runs.

*******************************************************************************************