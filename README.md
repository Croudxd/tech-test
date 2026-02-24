Question 1:
First problem was simple we just wasnt overloading the gov default variable, so simply added a check. Second was that there was trailing characters at the end of the strings, so i added a small function to check if there is, if so remove.

Question 2:
Okay that took me longer than it should have done, I was trying to get my head around what i was meant to be doing and then realised that we are basically just recreating the BondTradeLoader function but for the Fx data. After playing around ALOT i finally got it working, I think that i could maybe this could be turned into some sort of template to DRY, but i also think that this is fine and it works well, i think DRY might just cause more complexity than whats needed.

Question 3:
Third test passing all problems, this was slightly easier to me than the above problem, simple wrote a simple function that will parse the xml, Create a PricingEngineConfigItem with the data, and then we push that back onto the PricingEngineConfig vector.

Question 4:
Task 4 completed, just simply initalized and created the objects for the map, Also updated destructor to destroy all heap objects to avoid a leak.

I actually started to struggle on this question, could of been how late it is, but I struggled to know what was wanted, eventually asked AI and gave me an explanation of what to do. After having that was pretty simple on what i needed to do.

Question 5 and 6 (in the same commit oops):

So at this point I ramped up the AI usage lol, after using AI to understand what questiono 5 wanted and it initially being a bit challenging I had it working. I should know this but just never come across from my projects. This is something i would need to go over by myself just to get a better understanding, Would be a good idea to implement my own iterators for some of my own data structures.

6 was simple was just some conditionals and prints.

Question 7:
Question 8:
