const char mainPage[] = R"~~(
<!doctype html>
<html lang="en">

<head>
  <title>VU Meter</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <style>
    body {
      background-color: #323232;
    }

    section {
      text-align: center;
    }

    canvas {
      padding: 1em;
      margin: 0em;
    }
  </style>
</head>

<body>
  <section class="main">
    <canvas id="demo" width="100" height="400" data-val="">No canvas</canvas>
  </section>

  <script>


let vall = 0
    function vumeter(elem, config) {

      // Settings
      var max = config.max || 8;
      var boxCount = config.boxCount || 10;
      var boxCountRed = config.boxCountRed || 2;
      var boxCountYellow = config.boxCountYellow || 3;
      var boxGapFraction = config.boxGapFraction || 0.2;
      var jitter = config.jitter || 0.02;

      // Colours
      var redOn = 'rgba(255,47,30,0.9)';
      var redOff = 'rgba(64,12,8,0.9)';
      var yellowOn = 'rgba(255,215,5,0.9)';
      var yellowOff = 'rgba(64,53,0,0.9)';
      var greenOn = 'rgba(53,255,30,0.9)';
      var greenOff = 'rgba(13,64,8,0.9)';

      // Derived and starting values
      var width = elem.width;
      var height = elem.height;
      var curVal = 0;

      // Gap between boxes and box height
      var boxHeight = height / (boxCount + (boxCount + 1) * boxGapFraction);
      var boxGapY = boxHeight * boxGapFraction;

      var boxWidth = width - (boxGapY * 2);
      var boxGapX = (width - boxWidth) / 2;

      // Canvas starting state
      var c = elem.getContext('2d');

      // Main draw loop
      var draw = function () {


        var targetVal = parseInt(elem.dataset.val, 10);

        // Gradual approach
        if (curVal <= targetVal) {
          curVal += (targetVal - curVal) / 5;
        } else {
          curVal -= (curVal - targetVal) / 5;
        }

        vall = targetVal
        // Apply jitter
        if (jitter > 0 && curVal > 0) {
          var amount = (Math.random() * jitter * max);
          if (Math.random() > 0.5) {
            amount = -amount;
          }
          curVal += amount;
        }
        if (curVal < 0) {
          curVal = 0;
        }

        c.save();
        c.beginPath();
        c.rect(0, 0, width, height);
        c.fillStyle = 'rgb(32,32,32)';
        c.fill();
        c.restore();
        drawBoxes(c, targetVal);


        requestAnimationFrame(draw);
      };
      
      setInterval(() => {
        for (var i = 0; i < boxCount; i++) {
          if(isOn(i, vall)) beep()
        }
      }, 1000)

      // Draw the boxes
      function drawBoxes(c, val) {
        c.save();
        c.translate(boxGapX, boxGapY);
        for (var i = 0; i < boxCount; i++) {
          var id = getId(i);

          c.beginPath();
          if (isOn(id, val)) {
            c.shadowBlur = 10;
            c.shadowColor = getBoxColor(id, val);
          }
          c.rect(0, 0, boxWidth, boxHeight);
          c.fillStyle = getBoxColor(id, val);
          c.fill();
          c.translate(0, boxHeight + boxGapY);
        }
        c.restore();
      }

      // Get the color of a box given it's ID and the current value
      function getBoxColor(id, val) {
        // on colours
        if (id > boxCount - boxCountRed) {
          return isOn(id, val) ? redOn : redOff;
        }
        if (id > boxCount - boxCountRed - boxCountYellow) {
          return isOn(id, val) ? yellowOn : yellowOff;
        }
        return isOn(id, val) ? greenOn : greenOff;
      }

      function getId(index) {
        // The ids are flipped, so zero is at the top and
        // boxCount-1 is at the bottom. The values work
        // the other way around, so align them first to
        // make things easier to think about.
        return Math.abs(index - (boxCount - 1)) + 1;
      }

      function isOn(id, val) {
        // We need to scale the input value (0-max)
        // so that it fits into the number of boxes
        var maxOn = Math.ceil((val / max) * boxCount);
        return (id <= maxOn);
      }

      // Trigger the animation
      draw();
    }

    function beep() {
      var snd = new Audio("data:audio/wav;base64,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qJrx4KOQjahZxWbcZgztj2c49nKmkId44S71j0c8eV9yDK6uPRzx5X18eDvjvQ6yKo9ZSS6l//8elePK/Lf//IInrOF/FvDoADYAGBMGb7FtErm5MXMlmPAJQVgWta7Zx2go+8xJ0UiCb8LHHdftWyLJE0QIAIsI+UbXu67dZMjmgDGCGl1H+vpF4NSDckSIkk7Vd+sxEhBQMRU8j/12UIRhzSaUdQ+rQU5kGeFxm+hb1oh6pWWmv3uvmReDl0UnvtapVaIzo1jZbf/pD6ElLqSX+rUmOQNpJFa/r+sa4e/pBlAABoAAAAA3CUgShLdGIxsY7AUABPRrgCABdDuQ5GC7DqPQCgbbJUAoRSUj+NIEig0YfyWUho1VBBBA//uQZB4ABZx5zfMakeAAAAmwAAAAF5F3P0w9GtAAACfAAAAAwLhMDmAYWMgVEG1U0FIGCBgXBXAtfMH10000EEEEEECUBYln03TTTdNBDZopopYvrTTdNa325mImNg3TTPV9q3pmY0xoO6bv3r00y+IDGid/9aaaZTGMuj9mpu9Mpio1dXrr5HERTZSmqU36A3CumzN/9Robv/Xx4v9ijkSRSNLQhAWumap82WRSBUqXStV/YcS+XVLnSS+WLDroqArFkMEsAS+eWmrUzrO0oEmE40RlMZ5+ODIkAyKAGUwZ3mVKmcamcJnMW26MRPgUw6j+LkhyHGVGYjSUUKNpuJUQoOIAyDvEyG8S5yfK6dhZc0Tx1KI/gviKL6qvvFs1+bWtaz58uUNnryq6kt5RzOCkPWlVqVX2a/EEBUdU1KrXLf40GoiiFXK///qpoiDXrOgqDR38JB0bw7SoL+ZB9o1RCkQjQ2CBYZKd/+VJxZRRZlqSkKiws0WFxUyCwsKiMy7hUVFhIaCrNQsKkTIsLivwKKigsj8XYlwt/WKi2N4d//uQRCSAAjURNIHpMZBGYiaQPSYyAAABLAAAAAAAACWAAAAApUF/Mg+0aohSIRobBAsMlO//Kk4soosy1JSFRYWaLC4qZBYWFRGZdwqKiwkNBVmoWFSJkWFxX4FFRQWR+LsS4W/rFRb/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////VEFHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU291bmRib3kuZGUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMjAwNGh0dHA6Ly93d3cuc291bmRib3kuZGUAAAAAAAAAACU=");
      snd.play();
    }




  </script>
  <script>
    window.onload = function () {
      var demo = document.getElementById('demo');
      vumeter(demo, {
        "boxCount": 8,
        "boxGapFraction": 0.25,
        "max": 8,
      });

      setInterval(async () => {
        const res = await fetch('readValeur');
        const value = await res.text();
        demo.setAttribute('data-val', value);
      }, 1000);
    };
  </script>

</body>

</html>
)~~";