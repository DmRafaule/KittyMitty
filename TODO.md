Implement(UI) ball controller for character                 [+]
Implement(L) creature generator                             [-+]
    Implement(UI) attace controlle for characteer           [+]
    Implement(UI) properly attcking interaction wt enemy    [+]
    Implement(G)  effect for attake                         [-+]
    Implement(L) interaction with enemies(another creature) [+]
Implement(CORE) free up memory from entity                  [+]
    Implement(L) interaction with enemy part                [+]
    Implement(L) those states like: {
        power of attacke    [-]
        power of deffence   [-]
        пробиваемость       [+]
        плотность плоти     [+]
        blood               [+]
        stamina             [+]
        immune system       [-]
        connected           [-]
    }
Implement(UI) ability visualize status of creatures         [+]
    Implement(UI) some border   [-]
    Implement(UI) smart moving(if creature too close to left window border move lable to right from creature) [-]
    Implement(UI) toggle pages for stats [-]
Implement(L) weapon class [+]{
    Implement(G) some animation for attacke [+]
    Implement(L) interaction with enemy(and his part) [+]
        Implement(L) method attacke [+]
        Implement(L) method defence [-]
        Implement(L) method parry   [-]
    Implement(L) stats for weapon  [+]{
        Penetration power [+]
        Cutting power [+]
        Strike power [+]
        Solidity    [+]
        Additional  [-]
    Implement spetial attacks [-]
    }
}
Implement(L) damage logic [-] <1>{
    Implement(L) damage status to part of body [+]
    Implement(L) damage status to organs       [+]
    Implement(L) choise, where to strike       [+]
    Implement(C) slow down animation for entire layer [-]
}
Implement(UI) Camera for hero [+]
Implement(UI) Test Level [-]{
    Implement(Ph) collisions [-]
    Implement(Ph) gravity [-]
} 
Implement(L) AI for enemies [-] <2>
Implement(L) armor class[-]
Implement(L) item class [-]
