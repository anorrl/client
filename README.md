# ANORRLClientSource
### What is this?
This is the client source for 2016E (March 16th 2016, at the time of writing this 10 year anniversary! :D ). This is the ANORRL fork, have a look around!

If you're daring, try make a mac build, an android build whatever!!

## How do I set this up?? (for windows)

Alright, first. Grab the Visual Studio 2012 iso (and install VS2012) from [here](https://repo.lambda.cam/en_visual_studio_professional_2012_x86_dvd_2262334.iso), this is my repository site (hoarders dream) so it should be fine!
It's up to you what Visual Studio version you use, for example I use VS2019!

### Ok so I got that installed what now?

Right. At the root of your drive (C:\ for example) create `Trunk2012`. In that folder you clone this repo into it, name it `Client`.

You need the libraries of course! So get them [here](https://setup.lambda.cam/Contribs.7z)! It's about 1GB compressed, 9GB uncompressed. You extract that into `Contribs` in the Trunk2012 folder.

Create the `Build` folder too, that's where all the building is gonna end up in.

**IMPORTANT**

You need to set the `CONTRIB_PATH` and `BUILD_PATH` system environment variables. This whole project is based on the C:\Trunk2012 layout but if you are confident you can do your own structure, but you'll be on your own with that.

<img width="394" height="436" alt="Environment Variables" src="https://github.com/user-attachments/assets/fec069fe-a614-4c17-955d-01573542f15c" />

```
This is an example of the two variables, this is my setup!
```


### Screenies to help you!
<img width="500" alt="Trunk2012" src="https://github.com/user-attachments/assets/19b17f8d-10da-4429-a5ce-9326825bff70" />

```
This is C:\Trunk2012.
```

<img width="500" alt="BuildFolder" src="https://github.com/user-attachments/assets/a403e418-3882-47df-b961-d217618e065c" />

```
This is C:\Trunk2012\Build. The folders within are generated automatically by Visual Studio (at least it SHOULD) they go like this C:/Trunk2012/Build/<Project>/<Platform>/Release/
```

<img width="500" alt="ClientFolder" src="https://github.com/user-attachments/assets/4edbb094-f300-4adb-b5b8-a81a09a3956a" />

```
This is C:\Trunk2012\Client. This is where you clone THIS repo to.
```

<img width="500" alt="Contribs" src="https://github.com/user-attachments/assets/9c6ff0ba-37b4-464c-af62-cbaa70a83e70" />

```
This is C:\Trunk2012\Contribs. This is where you extract that Contribs.7z archive into.
```
