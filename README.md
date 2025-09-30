# Library – C++ Project

This project implements a simple system for managing books and readers:

* **Class `Carte` (Book)** – title, author, year, genre + binary save/load.
* **Class `ExemplarCarte` (BookCopy)** – reference to a book and a unique code.
* **Class `Cititor` (Reader)** – manages a list of borrowed copies.

Features:

* Borrowing and returning books (`+=`, `-=` operators).
* Display information with the `<<` operator.
* Save and load `Carte` objects from binary files.


