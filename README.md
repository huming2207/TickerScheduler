# TickerScheduler
Simple scheduler for ESP8266 Arduino based on Ticker

### Initialization ###

```TickerScheduler(uint size); ```

| Param | Description |
| --- | --- |
|  size  |  Amount of task Tickers to initialize  |

**Example**:

```TickerScheduler ts(5)```


### Add task ###

```    boolean add(uint i, uint32_t period, tscallback_t f, uint8_t noOfRuns = 0, boolean shouldFireNow = false); ```

| Param | Description |
| --- | --- |
|  i  |  Task ID  |
| period  | Task execution period in ms  |
| f | Task callback |
| noOfRuns |  Defines a number of callback runs. Zero means unlimited |
| shouldFireNow|  ```true``` if you want to execute task right after first scheduler update or wait next scheduled start |

**Returns**:

```true``` - task added sucessfully

```false``` - task was not added

**Example**:

```ts.add(0, 3000, sendData)```

```ts.add(0, 3000, sendData, 5 ) // run a taks 5 times ```

```ts.add(0, 3000, std::bind(&class::member, this), 5, true ) // run a taks 5 times with binded ```this``` pointer ```

### Execute scheduler in ```loop()``` ###

``` ts.update() ```

### Remove task ###

```boolean remove(uint i)```

### Pause task ###

Detaches an event from ticker, but does not remove from a task list.

```boolean pause(uint i)```


### Unpause task ###

```boolean unpause(uint i)```

**Returns**:

```true``` - task removed/(un)paused sucessfully

```false``` - task was not removed/(un)paused

| Param | Description |
| --- | --- |
|  i  |  Task ID  |

### TODO ###
* Task callback parameters support
* ...
