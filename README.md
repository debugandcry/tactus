# **tactus** #
nope not vibe coded my good friends.

**tactus** is an audio player for everyone who cares about their listening, aiming to be:
- built for **privacy** by being **local-first**.
- **optimized**, **performant** and **responsive**, for even the average potato.
- and most importantly, **understandable**.

> the word "tactus" refers to the most fundamental pulse, beat, or unit in music. pronounced TAK-tuhs, like "tack-tus".

**the project is still in the experimental and tentative starting phase, currently at version 0.0.10.**

**tactus** will also have  three main versions for different audiences:
* **tactus** itself -> stable, trusty and default. **for everyday use, and the everyday user.**
* (tactus) **sonare** -> the more experimental, graphical, expressive, and atmospheric experience. **for the power user.**
* **tactus minimal** -> (average branding, i know.) lightweight, heavily optimized and TUI focused. supports almost all filetypes too. for use on all devices, old or new - and **for the simple user.**

this project is being made on a laptop from 2013, so i assure you, dear reader, that when i say its optimized, it's not a lazy afterthought. if **tactus** feels responsive on this, **tactus** should be the same or faster on current-day hardware, and very much runnable on older hardware too.

## **tactus** ##
the planned main audio player will target Windows and Android at first, and will eventually branch out to MacOS and the wider Apple ecosystem (well... most of it) too.
tactus will have a very intuitive and self-explanatory GUI - graphical user interface -  that is meant to be usable by all. the graphical user interface will be crafted in QT/QML. 

## **tactus minimal** ##
this facet of the project is built on a TUI - terminal user interface - which is, in most cases, lighter than a GUI. first variant being produced, because of ease and simplicity. 

## **sonare** ## 
sonare is extremely tactile and immersive audio software, with an array of visualizations, playback settings and tweaks, all for the more eager enthusiasts.  

> the word "sonare" means "to sound" or "to resound". pronounced "so-NAHR-eh". yup.

## **features?** ##
- this project aims to start supporting most audio filetypes and codecs too - to honor all the music collectors, and for all the audio that modern players have, sadly, abandoned. 

- **tactus** will be built using a modular approach, with interchangeable source files that can be compiled to create a unique version for the more avid, experimental user.

- **tactus** will, for all three versions, use a handcrafted and thoughtful GUI/TUI, that is deeply native to the system it's on. not a web browser cosplaying as a desktop app.

- at setup, **tactus** will ask you if the app should stay online or offline. it listens to your decision. but, "why online?", you ask? details below.

- **tactus** also ensures that the local music listeners don't miss out on their music. **metadata normalization/"name-sync"** is an optional feature, that is planned to be implemented to take music like, for example:

  1. "great gig sky pink floyd original"
  2. "thegreatgigintheskybyremixer"
  3. "The gREAT GIG iN tHe SKY og"

  and convert them into **unique one-time-only audio fingerprints on-device** (using Chromaprint), and **temporarily** compare them against large metadata/audio databases using **encrypted/privacy-conscious** methods (**you** choose which one, as the user, like MusicBrainz, AcoustID, Discogs...) to return to you probabilistic/maybe, maybe-not suggestions on what version of a song you're listening to. the listed songs, in the example, would get converted to (on your word and permission, of course):

  1. "The Great Gig In The Sky"
  2. "The Great Gig In The Sky, Pulse - 1994"
  3. "The Great Gig In The Sky", and duplicate detection too, because it found a song uncannily like the first one.

  metadata normalization/"name-sync" will be a **completely optional feature**.

- **tactus** will also have a small, spotify-wrapped-like experience, with a few unique templates that show you what music you enjoyed and listened to the most - **ALL generated and done on your device, with this as an *optional* feature**. named "aevum". new templates and themes delivered via updates, which are, again, optional.

  > the word "aevum" refers to an age, an aeon or an era. pronounced "EE-vum". pretty fitting for a year-end recap.