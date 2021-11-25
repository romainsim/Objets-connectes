const char onHTML[];
const char offHTML[];
const char rgb_simpleHTML[];

onHTML = R"(
    
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>TEA</title>
            <style>
                .button {
                    background-color: #6d6a6a;
                    border: black;
                    color: black;
                    padding: 5px 5px;
                    text-align: center;
                    text-decoration: none;
                    display: inline-block;
                    font-size: 15px;
                    margin: 4px 2px;
                    cursor: pointer;
                }
            </style>
        </head>
        <body>
            <h2>Commande LED</h2>
            <p>La LED est allumée</p>
            <a href="on.html" class="button">Allumer</a><a href="off.html" class="button">Eteindre</a>
        </body>
        </html>
    
    )";

offHTML = R"(
    
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <title>TEA</title>
            <style>
                .button {
                    background-color: #6d6a6a;
                    border: black;
                    color: black;
                    padding: 5px 5px;
                    text-align: center;
                    text-decoration: none;
                    display: inline-block;
                    font-size: 15px;
                    margin: 4px 2px;
                    cursor: pointer;
                }
            </style>
        </head>
        <body>
            <h2>Commande LED</h2>
            <p>La LED est éteinte</p>
            <a href="on.html" class="button">Allumer</a><a href="off.html" class="button">Eteindre</a>
        </body>
        </html>
    
    )";

rgb_simple = R"(
    
           <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>TEA</title>
        <style>
            .button {
                background-color: #6d6a6a;
                border: black;
                color: black;
                padding: 5px 5px;
                text-align: center;
                text-decoration: none;
                display: inline-block;
                font-size: 15px;
                margin: 4px 2px;
                cursor: pointer;
            }
            label {
                font: 1rem 'Fira Sans', sans-serif;
            }
    
            input {
                margin: .4rem;
            }
        </style>
    </head>
    <body>
        <h2>Commande LED</h2>
        <a href="on.html" class="button">Allumer</a><a href="off.html" class="button">Eteindre</a>
        <form action="rgb_simple.html" method="get">
            <div>
                <label for="couleur">Couleur : </label>
                <input type="color" id="couleur" name="value">
            </div>
            <div>
                <input type="submit" value="Envoyer">
            </div>
        </form>
    </body>
    </html>
    
    )";
