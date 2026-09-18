# ANOther Roblox Retake Lol - client
## what is this?
This is the client source for ANORRL, it's based off the March 2016 source code (slowly but surely making it NOT roblox!). 

***THIS DOES NOT MEAN IT IS 2016 AT ALL.***

If you're daring, try make a mac build, a android build whatever!!

## contributors

Thanks to all these people for the contributions made to this project (of the client)

- typical_rodent for studio splash 18
- gammaguy for studio splash 19
- dosth for studio splash 34
- platos for studio splash 37
- glitchysavvy for studio splash 38

The rest were either made by me and/or power!

## how do I set this up?? (for windows)

Alright, first. Grab the Visual Studio 2012 iso (and install VS2012) from [here](https://archive.org/download/en_visual_studio_professional_2012_x86_dvd_2262334/en_visual_studio_professional_2012_x86_dvd_2262334.iso), this is the copy i use!
It's up to you what Visual Studio version you use after, for example I use VS2015! (Please be aware it has to be >= VS2012)

### ok so I got that installed what now?

Right. At the root of your drive (C:\ for example) create `dazai`. In that folder you clone this repo into it, name it `Client`.

You need the libraries of course! So get them [here](https://archive.org/details/anorrl-library.7z)! (*It's about ~640mb compressed, ~5.3GB uncompressed.*) You extract that into `Library` in the dazai folder.

Create the `Build` folder too, that's where all the building is gonna end up in.

**IMPORTANT**

You need to set the `CONTRIB_PATH` and `BUILD_PATH` system environment variables. This whole project is based on the C:\Trunk2012 layout but if you are confident you can do your own structure, but you'll be on your own with that.

<img title="Environment variables" width="394" height="436" alt="image" src="https://github.com/user-attachments/assets/6dc29968-b14c-4f28-99fb-89b46636fc9d" />

```
This is an example of the two variables, this is my setup!
```


### Screenies to help you!
<img width="500" alt="image" src="https://github.com/user-attachments/assets/d7f7ca75-0d0a-4009-bb29-477398c5160e" />

```
This is C:\dazai.
```

<img width="500" alt="image" src="https://github.com/user-attachments/assets/bde52819-01d8-4b43-9721-c6e48008cf14" />


```
This is C:\dazai\Build. The folders within are generated automatically by Visual Studio (at least it SHOULD) they go like this C:/dazai/Build/<Project>/<Platform>/Release/
```

<img width="500" alt="image" src="https://github.com/user-attachments/assets/fa4fa9d0-501f-41a2-85b1-1353ca3fe569" />


```
This is C:\dazai\Client. This is where you clone THIS repo to.
```

<img width="500" alt="image" src="https://github.com/user-attachments/assets/e24a299f-a909-4106-b61d-a29c75b4fb4e" />


```
This is C:\dazai\Library. This is where you extract that Library.7z archive into.
```
