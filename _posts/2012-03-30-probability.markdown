---
layout: post
title: "The correct way of doing probability"
date: 2012-03-30 16:48:14 -0400
tags: math
---
Reading an article from [DataGenetics](http://www.datagenetics.com/blog/april12012/index.html), which shows how people flawed when dealing with probability.

The 26 letters of English, in descending frequency of use, are

    ETAOIN SHRDLU CMFWYP VBGKQJ XZ

but the frequency is about an article. If the pool is changed, the frequency order changed as well. For example, the frequency order of letters in a dictionary file shall be

    ESIARN TOLCDU PMGHBY FVKWZX QJ

because dictionary of words do not have repeated words, but some words (e.g. articles and prepositions) are used very often in an article.

Consider hacking a hangman game, we should use the latter frequency instead of the former because we are dealing with words regardless its common-ness. However, we should also take the length of word into account, because e.g. one-letter word in English can only be "a" or "I" but not others. So we should have the frequency order of letters *subject to the condition* of word length. The table is given in the article above, copied here:

``` csv
,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20     
#1,A,A,A,A,S,E,E,E,E,E,E,E,I,I,I,I,I,I,I,I      
#2,I,O,E,E,E,S,S,S,S,I,I,I,E,E,E,E,E,S,E,O      
#3, ,E,O,S,A,A,I,I,I,S,S,S,N,T,T,T,T,E,T,E      
#4, ,I,I,O,R,R,A,A,R,R,N,N,T,S,N,S,N,T,O,T      
#5, ,M,T,I,O,I,R,R,A,A,A,T,S,N,S,N,S,O,N,R      
#6, ,H,S,R,I,O,N,N,N,N,R,A,A,A,O,A,O,N,A,S      
#7, ,N,U,L,L,L,T,T,T,T,T,R,O,O,A,O,A,R,S,A      
#8, ,U,P,T,T,N,O,O,O,O,O,O,R,R,R,R,R,A,R,N      
#9, ,S,R,N,N,T,L,L,L,L,L,L,L,L,L,L,L,L,L,C      
#10, ,T,N,U,U,D,D,D,C,C,C,C,C,C,C,C,C,C,C,L     
#11, ,Y,D,D,D,U,U,C,D,D,U,P,P,P,P,P,P,P,P,P     
#12, ,B,B,P,C,C,C,U,U,U,D,U,U,U,U,U,U,M,M,H     
#13, ,L,G,M,Y,M,G,G,G,G,P,M,M,M,M,M,M,U,U,U     
#14, ,P,M,H,P,P,P,M,M,M,M,D,G,D,D,H,H,H,H,M     
#15, ,X,Y,C,M,G,M,P,P,P,G,G,D,H,H,D,D,D,D,Y     
#16, ,D,L,B,H,H,H,H,H,H,H,H,H,G,G,Y,G,G,G,D     
#17, ,F,H,K,G,B,B,B,B,B,B,Y,Y,Y,Y,G,Y,Y,Y,G     
#18, ,R,W,G,B,Y,Y,Y,Y,Y,Y,B,B,B,B,B,B,B,B,B     
#19, ,W,F,Y,K,K,F,F,F,F,F,V,V,V,V,V,V,V,V,Z     
#20, ,G,C,W,F,F,K,K,V,V,V,F,F,F,F,F,F,Z,F,V     
#21, ,J,K,F,W,W,W,W,K,K,K,Z,Z,Z,Z,Z,Z,F,Z,F     
#22, ,K,X,V,V,V,V,V,W,W,W,K,X,X,X,X,X,X,X,K     
#23, , ,V,J,Z,Z,Z,Z,Z,Z,Z,W,K,K,W,W,Q,Q,K,X     
#24, , ,J,Z,X,X,X,X,X,X,X,X,W,W,K,Q,W,W,J,J     
#25, , ,Z,X,J,J,J,Q,Q,Q,Q,Q,Q,Q,Q,K,J,K,Q,Q     
#26, , ,Q,Q,Q,Q,Q,J,J,J,J,J,J,J,J,J,K, ,W,      
```

Which means, for example, no 20-letter words in English contains a "W".

Further this game is that, the above table is only for the first letter in the hangman game. If we revealed some letters in the game, the correct strategy of guessing changes. This can taken into account of the correlation of letters in and not in the word, as well as the position of revealed letters of the word.
