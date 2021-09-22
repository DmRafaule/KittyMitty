# Fight system

## Consist of next modules

   ***Weapon***

Must have:

* Attack detection [-]
* Deffend detection [-]
* Change stats for owner's weapon [+]
* Change stats for target [+]
* Can set new weapon type(Sword, Spear etc.) [-]
* Can unset weapon [+]
* Two type of weapon (Melee, Ranged)
  * For melee:
    * Sword(stock) [-]
    * Spear(stock) [-]
    * Tomogavk [-]
    * Axe [-]
    * LongSword [-]
    * Zweihander [-]
    * Halberd [-]
    * Dagger [-]
  * For ranged
    * Bow [-]
    * Crossbow [-]

   ***Armor***

* Can set and unset new armor
* Three type of armor (ForTop,ForMiddle,ForBottom)
  * For top
    * Guardian helm [-]
    * Guardian commandor helm [-]
  * For Middle
    * Guardian cuirassa [-]
    * Guardian commandor cuirassa [-]
  * For bottom
    * Guardian boots [-]
    * Guardian commandor boots [-]

   ***Body***

* Three type of part body (Top(head),Middle(torso),Bottom(legs))
  * For top:
    * First hit -> lose orientation, and player, bleeding [-]
    * Second hit -> death [-]
  * For middle:
    * First hit -> lose ability for hiting, big bleeding [-]
    * Second hit -> lose weapon, big big bleeding [-]
    * Third hit  -> death [-]
  * For bottom:
    * First hit -> shrink max speed, and ability for jump, bleeding [-]
    * Second hit -> cant move, bleeding [-]
* Blood losing [-]
* Animation for diffrent type dammage [-]
* Remove organs [+]

   ***AI battle***

* Approach to other creature [+]
* Must able to attake by some patterns [+]

   ***AI for bosses***

* Switching between phases [-]
* Super attacks [-]

   ***Additional***
* Reimplement running [-]