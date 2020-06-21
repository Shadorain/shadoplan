# shadoplan
A very handy program that handles tasks the simplest but just as reasonable way

### Concepts
* [X] Will be written in C, will be patchable and will have a config.h file
* [X] Will handle adding new tasks quickly (will ask for type of TODO; option to do via direct typing into command or run command then prompt for it)
* [ ] Can make new TODO direcories? def new topics
* [X] Store completed TODO's with date, in separate file
* [X] Should have unlimited number of TODO files that can be made for different broad categories (by user)
* [X] Allow for uncategorized, unprioritized, or no due date entries?

### Options (Sections---> t:TODOs, c:Categories, cpl:Completed)
* [X] sp t [-a/--add] <Title> <Description> <Date> <Priority> <Category> 
    * (Pop open menu of all categories)
    * Title
    * Description
    * Category (Parent)
* [ ] sp t [-d/--del] <Title> 
    * (Should pop open a menu of all existing TODOs with numbers? TAB COMPLETE)
    * ID
* [O] sp t [-l/--list]
    * (Will just cat out a list of all TODOs and their trees in a pretty way) 
* [ ] sp t [-s/--set] <FILE>
    * (Set TODO List to look at) 

### TODO's
* HELP MENU
    * options should be able to be manually done or interactively
