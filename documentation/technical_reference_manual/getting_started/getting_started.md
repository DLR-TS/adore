<!--
********************************************************************************
* Copyright (C) 2017-2020 German Aerospace Center (DLR). 
* Eclipse ADORe, Automated Driving Open Research https://eclipse.org/adore
*
* This program and the accompanying materials are made available under the 
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0 
*
* Contributors: 
*   Andrew Koerner 
********************************************************************************
-->
This guide will help you get your system set up and configured to run ADORe.

1. First review the [System Requirements 🔗](system_requirements.md). 

2. Next review the [Prerequisites 🔗](prerequisites.md) 

3. After installing GNU Make and Docker you are ready to [clone the repository 🔗](cloning_adore.md) 

5. Build ADORe and ADORe CLI with the [Building ADORe 🔗](building_adore.md) guide 

After cloning and satisfying all system prerequisites and building ADORe
you can start the ADORe CLI interactive shell docker context
To do this navigate to the root of the ADORe repository directory
and run the following command:
```bash
make cli
```

> **✅ SUCCESS:**
> If you are greeted with the following ADORe CLI car then you have successfully setup ADORe:
```
            ____ 
         __/  |_\__
        |           -. 
  ......'-(_)---(_)--' 
```


> **✅ INFO:** Next steps...
