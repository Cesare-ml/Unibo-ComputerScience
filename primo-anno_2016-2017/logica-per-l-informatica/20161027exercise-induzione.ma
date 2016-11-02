(**************************************************************************)
(*       ___	                                                          *)
(*      ||M||                                                             *)
(*      ||A||       A project by Andrea Asperti                           *)
(*      ||T||                                                             *)
(*      ||I||       Developers:                                           *)
(*      ||T||       A.Asperti, C.Sacerdoti Coen,                          *)
(*      ||A||       E.Tassi, S.Zacchiroli                                 *)
(*      \   /                                                             *)
(*       \ /        This file is distributed under the terms of the       *)
(*        v         GNU Lesser General Public License Version 2.1         *)
(*                                                                        *)
(**************************************************************************)

(* Esercizio 0
   ===========

   Compilare i seguenti campi:

   Nome1: ...
   Cognome1: ...
   Matricola1: ...
   Account1: ...

   Nome2: ...
   Cognome2: ...
   Matricola2: ...
   Account2: ...

   Prima di abbandonare la postazione:

   * compilare il questionario in fondo al file

   * salvare il file (menu `File ▹ Save as ...`) nella directory (cartella)
     /public/ con nome linguaggi_Account1.ma, ad esempio Mario Rossi, il cui
     account è mrossi, deve salvare il file in /public/linguaggi_mrossi.ma

   * mandatevi via email o stampate il file. Per stampare potete usare
     usare l'editor gedit che offre la funzionalità di stampa
*)



(* ATTENZIONE
   ==========

   Quanto segue sono definizioni di tipi di dato/grammatiche e funzioni
   definite per ricorsione strutturale prese dall'esercitazione della volta
   scorsa. Non cambiarle e procedere con i nuovi esercizi di dimostrazione
   che sono intervallati con le definizioni.
*)

include "logic/equality.ma".

(* nat ::= "O" | "S" nat   *)
inductive nat : Type ≝
   O : nat
 | S : nat → nat.

(* list_nat ::= "Nil" | "Cons" nat list_nat *)
inductive list_nat : Type ≝
   Nil : list_nat
 | Cons : nat → list_nat → list_nat.

(* tree_nat ::= "Leaf" nat | "Node" nat nat *)
inductive tree_nat : Type ≝
   Leaf : nat → tree_nat
 | Node : tree_nat → tree_nat → tree_nat.

(*
  plus O m = m
  plus (S x) m = S (plus x m)
*)
let rec plus n m on n ≝
 match n with
 [ O ⇒ m
 | S x ⇒ S (plus x m) ].

(* Esercizio 1
   ===========

   Dimostrare l'associatività della somma per induzione strutturale su x.
*)
theorem plus_assoc: ∀x,y,z. plus x (plus y z) = plus (plus x y) z.
 (* Possiamo iniziare fissando una volta per tutte le variabili x,y,z.
    A lezione vedremo il perchè. *)
 assume x : nat.
 assume y : nat.
 assume z : nat.
 we proceed by induction on x to prove (plus x (plus y z) = plus (plus x y) z).
 case O.
  (* Scriviamo cosa deve essere dimostrato e a cosa si riduce eseguendo le
     definizioni. *)
  we need to prove (plus O (plus y z) = plus (plus O y) z)
  or equivalently (plus y z = plus y z).
  (* done significa ovvio *)
  done.
 case S (w: nat).
  (* Chiamiamo l'ipotesi induttiva IH e scriviamo cosa afferma.
     Ricordate: altro non è che la chiamata ricorsiva su w. *)
  by induction hypothesis we know (plus w (plus y z) = plus (plus w y) z) (IH).
  we need to prove (plus (S w) (plus y z) = (plus (plus (S w) y) z))
  or equivalently (S (plus w (plus y z)) = S (plus (plus w y) z)).
  (* by IH done significa ovvio considerando l'ipotesi IH *)
  by IH done.
qed.


(* La funzione `sum` che, data una `list_nat`, calcola la
   somma di tutti i numeri contenuti nella lista. *)
let rec sum L on L ≝
 match L with
 [ Nil ⇒ O
 | Cons N TL ⇒ plus N (sum TL)
 ].

(* La funzione binaria `append` che, date due `list_nat` restituisca la
   `list_nat` ottenuta scrivendo in ordine prima i numeri della prima lista in
   input e poi quelli della seconda.
*)
let rec append L1 L2 on L1 ≝
 match L1 with
 [ Nil ⇒ L2
 | Cons HD TL ⇒ Cons HD (append TL L2)
 ].

(* Esercizio 2
   ===========

   La somma di tutti i numeri nella concatenazione di due liste è uguale
   alla somma delle somme di tutti i numeri nelle due liste. *)
theorem sum_append: ∀L1,L2. sum (append L1 L2) = plus (sum L1) (sum L2).
 assume L1 : list_nat.
 assume L2 : list_nat.
 we proceed by induction on L1 to prove (sum (append L1 L2) = plus (sum L1) (sum L2)).
 case Nil.
  we need to prove (sum (append Nil L2)=plus (sum Nil) (sum L2))
  or equivalently (sum L2 = sum L2).
  done.
 case Cons (N: nat) (L: list_nat).
  by induction hypothesis we know (sum (append L L2)=plus (sum L) (sum L2)) (IH).
  we need to prove (sum (append (Cons N L) L2)=plus (sum (Cons N L)) (sum L2))
  or equivalently ( plus N (sum (append L L2)) =  (plus (plus N (sum L)) (sum L2))).
  (* ( (plus N (plus (sum L) (sum L2))) = (plus N (plus (sum L) (sum L2)))) *)
  (* Per concludere servono sia l'ipotesi induttiva IH che il teorema plus_assoc
     dimostrato prima. Convincetevene.

     ( S (sum (append L L2)) = S (plus (sum L) (sum L2)))

     Nota: se omettete IH, plus_assoc o entrambi Matita ci riesce lo stesso.
     Rendere stupido un sistema intelligente è complicato... Tuttavia non
     abusatene: quando scrivete done cercate di avere chiaro perchè il teorema
     è ovvio e se non vi è chiaro, chiedete. *)
  by IH, plus_assoc done.
qed.

(* La funzione `plusT` che, dato un `tree_nat`, ne restituisce la
   somma di tutte le foglie. *)
let rec plusT T on T ≝
 match T with
 [ Leaf n ⇒ n
 | Node T1 T2 ⇒ plus (plusT T1) (plusT T2)
 ].

(* La funzione `visit` che, dato un `tree_nat`, calcola la `list_nat`
   che contiene tutti i numeri presenti nelle foglie dell'albero in input,
   nell'ordine in cui compaiono nell'albero da sinistra a destra.
*)
let rec visit T on T ≝
 match T with
 [ Leaf N ⇒ Cons N Nil
 | Node T1 T2 ⇒ append (visit T1) (visit T2)
 ].

(* Esercizio 3
   ===========

   Iniziare a fare l'esercizio 4, commentando quel poco che c'è dell'esercizio 3.
   Nel caso base vi ritroverete, dopo la semplificazione, a dover dimostrare un
   lemma non ovvio. Tornate quindi all'esercizio 3 che consiste nell'enunciare e
   dimostrare il lemma. *)

lemma plus_O: ∀N. plus N O = N.
 assume N : nat.
 we proceed by induction on N to prove (plus N O = N).
 case O.
  we need to prove (plus O O=O)
  or equivalently (O = O).
 done.
 case S (N : nat).
  by induction hypothesis we know (plus N O = N) (IH).
  we need to prove (plus (S N) O = S N)
  or equivalently (S (plus N O)=S N).
 done.
qed.


(* Esercizio 4
   ===========

   Dimostriamo che la `plusT` è equivalente a calcolare la `sum` sul risultato
   di una `visit`. *)


theorem plusT_sum_visit: ∀T. plusT T = sum (visit T).
 assume T : tree_nat.
 we proceed by induction on T to prove (plusT T = sum (visit T)).
 case Leaf (N : nat).
  we need to prove (plusT (Leaf N)=sum (visit (Leaf N)))
  or equivalently (N = plus N O).
  (*
  Ciò che dobbiamo dimostrare non è ovvio (perchè?). Per proseguire,
     completate l'esercizio 3 enunciando e dimostrando il lemma che vi serve.
     Una volta risolto l'esercizio 3, questo ramo diventa ovvio usando il lemma.*)
  by plus_O done.
 case Node.
  (* In questo caso siamo obbligati a elencare gli argomenti T1 e T2 di Node
     uno alla volta usando assume perchè dopo ognuno il sistema ci obbliga ad
     assumere la relativa ipotesi induttiva. *)
  assume T1 : tree_nat.
  by induction hypothesis we know (plusT T1 = sum (visit T1)) (IH1).
  assume T2 : tree_nat.
  by induction hypothesis we know (plusT T2 = sum (visit T2)) (IH2).
  we need to prove (plusT (Node T1 T2)=sum (visit (Node T1 T2)))
  or equivalently (plus (plusT T1) (plusT T2) = sum (append (visit T1) (visit T2))) .
  (*

    (plusT (Node T1 T2)                     = sum (visit (Node T1 T2)))
    (plus  (plusT T1) (plusT T2)            = sum (append (visit T1) (visit T2)))
    (plus (sum (visit T1)) (sum (visit T2)) = sum (append (visit T1) (visit T2)))
    (plus (sum (visit T1)) (sum (visit T2)) = plus (sum (visit T1)) (sum (visit T2)))
  *)
  (* Oltre alla due ipotesi induttive, di quale altro lemma dimostrato in
     precedenza abbiamo bisogno per concludere la prova?*)
  by IH1,IH2,sum_append done.
qed.

(* Un altro modo di calcolare la somma di due numeri: per ricorsione strutturale
   sul secondo argomento.

   plus' m O = m
   plus' m (S x) = S (plus' m x)
*)
let rec plus' m n on n ≝
 match n with
 [ O ⇒ m
 | S x ⇒ S (plus' m x) ].

(* Esercizio 5:
   ============

   Dimostriamo l'equivalenza dei due metodi di calcolo.
   Vi servirà un lemma: capite quale e dimostratelo.
*)

lemma s_plus_x: ∀x,y. plus x (S y) = S (plus x y).
  assume x : nat.
  assume y : nat.
  we proceed by induction on x to prove (plus x (S y)=S (plus x y)).
  case O.
    we need to prove (plus O (S y)=S (plus O y))
    or equivalently (S y = S y).
  done.
  case S (N : nat).
    by induction hypothesis we know (plus N (S y)=S (plus N y)) (IH).
    we need to prove (plus (S N) (S y)=S (plus (S N) y))
    or equivalently (S (plus N (S y)) = S (S (plus N y))).
    by IH done.
qed.

theorem plus_plus': ∀x,y. plus x y = plus' x y.
 (* Nota: la dimostrazione è più facile se andate per induzione su y perchè
    potrete riciclare un lemma già dimostrato.
    Se andate per induzione su x vi verrà lo stesso, ma in tal caso avrete
    bisogno di due lemmi, ognuno dei quali non ancora dimostrati. *)
  assume x : nat.
  assume y : nat.
  we proceed by induction on y to prove (plus x y=plus' x y).
  case O.
   we need to prove (plus x O=plus' x O)
   or equivalently (plus x O = x).
    (*
    Ciò che dobbiamo dimostrare non è ovvio (perchè?). Per proseguire,
       completate l'esercizio 3 enunciando e dimostrando il lemma che vi serve.
       Una volta risolto l'esercizio 3, questo ramo diventa ovvio usando il lemma.*)
   by plus_O done.
   case S (N : nat).
    by induction hypothesis we know (plus x N=plus' x N) (IH).
    we need to prove (plus x (S N)=plus' x (S N))
    or equivalently (plus x (S N)=S (plus' x N)).
    by IH,s_plus_x done.
  qed.

(* Esercizio 6,7,8,…
   =================

   Volete esercitarvi a casa su altre dimostrazioni facili come queste?
   Ecco un buono spunto: definite la funzione che inserisce un numero in
   coda a una lista e usatela per definire la funzione rev che restituisce
   la lista ottenuta leggendo la lista in input dalla fine all'inizio.
   Esempio: rev (Cons 1 (Cons 2 (Cons 3 Nil))) = (Cons 3 (Cons 2 (Cons 1 Nil))).
   Poi dimostrate che ∀L. sum (rev L) = sum L. Per riuscirci vi serviranno una
   cascata di lemmi intermedi da enunciare e dimostrare. *)

(* Esercizio 6 *)
(* inserisce un numero in coda a una lista *)
let rec append_n L N on L ≝
 match L with
 [ Nil ⇒ Cons N Nil
 | Cons x l ⇒ Cons x (append_n l N )
 ].

(* reverse di una lista *)
let rec rev L on L ≝
 match L with
 [ Nil ⇒ Nil
 | Cons n l ⇒ append_n (rev l) n
 ].

lemma plus_additiva: ∀x,y. plus x y = plus' y x.
  assume x : nat.
  assume y : nat.
  we proceed by induction on x to prove (plus x y=plus' y x).
  case O.
    we need to prove (plus O y=plus' y O)
    or equivalently (y = y).
    done.
  case S (N : nat).
    by induction hypothesis we know (plus N y=plus' y N)(IH).
    we need to prove (plus (S N) y=plus' y (S N))
    or equivalently (S (plus N y) = S (plus' y N)).
    done.
  qed.

lemma plus_mix: ∀x,y,z. plus x (plus y z) = plus y (plus x z).
  assume x : nat.
  assume y : nat.
  assume z : nat.
  we proceed by induction on x to prove (plus x (plus y z) = plus y (plus x z)).
  case O.
    we need to prove (plus O (plus y z)=plus y (plus O z))
    or equivalently (plus y z = plus y z).
    done.
  case S (N : nat).
    by induction hypothesis we know (plus N (plus y z)=plus y (plus N z))(IH).
    we need to prove (plus (S N) (plus y z)=plus y (plus (S N) z))
    or equivalently (S (plus N (plus y z)) = plus y (S(plus N z))).
    by IH,plus_plus',plus_assoc,plus_additiva done.
qed.

lemma sum_append_n: ∀L,N. sum (append_n L N ) = plus N (sum L).
  assume L : list_nat.
  assume N : nat.
  we proceed by induction on L to prove (sum (append_n L N)=plus N (sum L)).
  case Nil.
    we need to prove (sum (append_n Nil N)=plus N (sum Nil))
    or equivalently (plus N O = plus N O).
    done.
  case Cons (M : nat) (K : list_nat).
    by induction hypothesis we know (sum (append_n K N)=plus N (sum K))(IH).
    we need to prove (sum (append_n (Cons M K) N)=plus N (sum (Cons M K)))
    or equivalently (plus M (sum (append_n K N))=plus N (plus M (sum K))).
    by IH,plus_mix done.
qed.

(*
              (plus M (sum (append_n K N)) = plus N (plus M (sum K))
IH            (plus M (plus N (sum K))     = plus N (plus M (sum K)))
plus_assoc    (plus (plus M N) (sum K)     = plus (plus N M) (sum K))
plus_plus'    (plus (plus M N) (sum K)     = plus (plus' N M) (sum K))
plus_additiva (plus (plus M N) (sum K)     = plus (plus M N) (sum K))

*)

theorem sum_rev: ∀L. sum (rev L) = sum L.
  assume L : list_nat.
  we proceed by induction on L to prove (sum (rev L)=sum L).
  case Nil.
    we need to prove (sum (rev Nil)=sum Nil)
    or equivalently (O=O).
    done.
  case Cons (N : nat) (L1 : list_nat).
    by induction hypothesis we know (sum (rev L1)=sum L1)(IH).
    we need to prove (sum (rev (Cons N L1))=sum (Cons N L1))
    or equivalently (sum (append_n (rev L1) N) = plus N (sum L1)).
    by IH, sum_append_n done.
qed.

(* Questionario

   Compilare mettendo una X nella risposta scelta.

   1) Pensi che sia utile l'integrazione del corso con una attività di
      laboratorio?

      [ ] per niente        [ ] poco     [ ] molto


   2) Pensi che gli esercizi proposti ti siano stati utili a capire meglio
      quanto visto a lezione?

      [ ] per niente        [ ] poco     [ ] molto


   3) Gli esercizi erano

      [ ] troppo facili     [ ] alla tua portata      [ ] impossibili


   4) Il tempo a disposizione è stato

      [ ] poco              [ ] giusto          [ ] troppo


   5) Cose che miglioreresti nel software Matita

      .........


   6) Suggerimenti sullo svolgimento delle attività in laboratorio

      .........


*)
