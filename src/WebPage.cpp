const char* webPage = R"HTMLPAGE(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>%PAGE_NAME%</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }

        .input-group {
            margin-bottom: 15px;
        }

        label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
        }

        input {
            padding: 10px;
            width: 100%%;
            max-width: 400px;
            box-sizing: border-box;
            border: 1px solid #ccc;
            border-radius: 5px;
        }

        .description {
            font-size: 0.9em;
            color: #666;
            margin-top: 5px;
        }

        button {
            padding: 10px 20px;
            background-color: #4CAF50;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
        }

        button:hover {
            background-color: #45a049;
        }
    </style>
</head>
<body>
    <h1>%PAGE_NAME%</h1>
    <div id="form-container"></div>
    <button onclick="submitSettings()">speichern</button>

    <script>
        async function loadSettings() {
            try {
                // JSON-Daten von der Adresse "/settings" abrufen
                const response = await fetch('%SETTINGS_URL%');
                if (!response.ok) {
                    throw new Error('Fehler beim Laden der Einstellungen: ' + response.statusText);
                }

                const settings = await response.json();

                // Container für das Formular auswählen
                const formContainer = document.getElementById('form-container');

                // Für jedes Element im JSON-Array ein Eingabefeld erstellen
                settings.forEach(setting => {
                    const inputGroup = document.createElement('div');
                    inputGroup.classList.add('input-group');

                    const label = document.createElement('label');
                    label.textContent = setting.description;

                    const input = document.createElement('input');
                    input.type = 'text';
                    input.value = setting.value;
                    input.id = setting.key;

                    inputGroup.appendChild(label);
                    inputGroup.appendChild(input);

                    formContainer.appendChild(inputGroup);
                });
            } catch (error) {
                console.error('Fehler:', error);
                alert('Einstellungen konnten nicht geladen werden.');
            }
        }

        function submitSettings() {
            const formContainer = document.getElementById('form-container');
            const inputs = formContainer.querySelectorAll('input');
            const formData = Array.from(inputs).map(input => ({
                key: input.id,
                value: input.value
            }));            

            // PUT-Anfrage senden
            fetch('%SETTINGS_URL%', {
                method: 'PUT',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(formData)
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP-Fehler! Status: ${response.status}`);
                }
                return response.json();
            })
            .then(data => {
                console.log('Erfolgreich gesendet:', data);
            })
            .catch(error => {
                console.error('Fehler beim Senden der Anfrage:', error);
            });
        }

        document.addEventListener('DOMContentLoaded', loadSettings);
    </script>
</body>
</html>
)HTMLPAGE";
