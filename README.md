# shadoplan
A very handy program that handles tasks the simplest but just as reasonable way

### Concepts
* [X] Will be written in C, will be patchable and will have a config.h file
* [ ] Will handle adding new tasks quickly (will ask for type of TODO; option to do via direct typing into command or run command then prompt for it)
* [ ] Can make new TODO direcories? def new topics
* [ ] Store completed TODO's with date, in separate file
* [ ] Should have unlimited number of TODO files that can be made for different broad categories (by user)

### Options (Sections---> t:TODOs, c:Categories, cmpl:Completed)
* [ ] sp t [-a/--add] <Title> <Description> <Date> <Category> 
    * (Pop open menu of all categories)
    * Title
    * Description
    * Category (Parent)
* [ ] sp t [-d/--del] <Title> 
    * (Should pop open a menu of all existing TODOs with numbers? TAB COMPLETE)
    * ID
* [ ] sp t [-c/--cpl] <Title>
    * (Should open a menu for existing TODOs, TAB COMPLETE NEEDS TO EXIST)
    * ID
* [ ] sp t [-l/--list]
    * (Will just cat out a list of all TODOs and their trees in a pretty way) 

### TODO's
* HELP MENU
    * options should be able to be manually done or interactively
