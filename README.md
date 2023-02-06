# Mini-projet C++
Chenrui ZHU, Xiangyong LI, Xianxiang ZHANG, Mamisoa RANDRIANARIMANANA

## Introduction
Une géneration dynamique de la carte

Une stratégie hybride, basée sur la stratégies basique - StrategyBasic.dll et la stratégie de la différence de nombre de dés - StrategyDiffDice.dll

## Compilation
Ouvrir .sln dan Visual Studio 2022 et générer des solutions.

Nous utilisons aussi cppcheck pour vérifier notre code.
```
cppcheck GenMap/ 2> err.txt 
cppcheck StrategyDummy/ 2> err.txt
```

## Exécution
Le code suivant doit être exécuté à partir de la racine du projet sous PowerShell ou ubuntu. GenMap.dll et StrategyDummy.dll sont générés par défaut avec la génération de la carte et la stratégie déjà modifiés. 

J'ai nommé les .dll générées par différentes stratégies, de sorte que StrategyDummy.dll est egal à StrategyMix.dll.

### Stratégie hybride contre stratégie test
sur PowerShell ou Ubuntu
```
./bin/x64/Debug/DiceWars.exe -s ./bin/x64/Debug/StrategyMix.dll -s ./bin/x64/Debug/Strategy.dll -r ./bin/x64/Debug/Referee.dll -m ./bin/x64/Debug/GenMap.dll -g ./bin/x64/Debug/Gui.dll

ou

./bin/x64/Debug/DiceWars.exe -s ./bin/x64/Debug/StrategyDummy.dll -s ./bin/x64/Debug/Strategy.dll -r ./bin/x64/Debug/Referee.dll -m ./bin/x64/Debug/GenMap.dll -g ./bin/x64/Debug/Gui.dll
```
sur cmd
```
bin\x64\Debug\DiceWars.exe -s bin\x64\Debug\StrategyDummy.dll -s bin\x64\Debug\Strategy.dll -r bin\x64\Debug\Referee.dll -m bin\x64\Debug\GenMap.dll -g bin\x64\Debug\Gui.dll
```

### 4 Stratégies ensemble
sur PowerShell ou Ubuntu
```
./bin/x64/Debug/DiceWars.exe -s ./bin/x64/Debug/StrategyMix.dll -s ./bin/x64/Debug/StrategyDiffDice.dll -s ./bin/x64/Debug/StrategyBasic.dll -s ./bin/x64/Debug/Strategy.dll -r ./bin/x64/Debug/Referee.dll -m ./bin/x64/Debug/GenMap.dll -g ./bin/x64/Debug/Gui.dll

ou

./bin/x64/Debug/DiceWars.exe -s ./bin/x64/Debug/StrategyDummy.dll -s ./bin/x64/Debug/StrategyDiffDice.dll -s ./bin/x64/Debug/StrategyBasic.dll -s ./bin/x64/Debug/Strategy.dll -r ./bin/x64/Debug/Referee.dll -m ./bin/x64/Debug/GenMap.dll -g ./bin/x64/Debug/Gui.dll
```
sur cmd
```
bin\x64\Debug\DiceWars.exe -s bin\x64\Debug\StrategyMix.dll -s bin\x64\Debug\StrategyDiffDice.dll -s bin\x64\Debug\StrategyBasic.dll -s bin\x64\Debug\Strategy.dll -r bin\x64\Debug\Referee.dll -m bin\x64\Debug\GenMap.dll -g bin\x64\Debug\Gui.dll
```

Vous pouvez également exécuter les fichiers dans ./Release, mais il semble que la version release ne soit pas stable.

## Test
Ouvrir en parallèle dans Google chrome la page localhost:5678.
