# Monitoramento de Nível de Água

Projeto feito como Trabalho de Conclusão de curso para a Graduação de Ciência da Computação na Universidade Federal Fluminense.

- Aluno: Breno Reis dos Santos
- Orientador: Raphael Guerra

---

## Sumário
---

- [Propósito](#propósito)
- [Instalação](#instalação)
  - [VSCode](#vscode)
- [Como Utilizar](#como-utilizar)
  - [Variáveis](#variáveis)
  - [Constantes](#contantes)


## Propósito
---

O projeto tem como objetivo realizar o monitoramento do nível da água de 1 Cisterna e 4 Caixas d'água no mesmo nível. Esse monitoramento escreve registros em um arquivo que será usado para gerar relatórios mostrando o histórico de abastecimento de água na casa.

## Instalação
---
## VSCode

### Build
Antes de qualquer coisa, instale as extensões: C/C++ e PlatformIO.

- Com a extensão do PlatformIO instalada, aparecerá diversas opções na barra de tarefas inferior. Uma delas é o símbolo de "✅", que fará o build. Clique nele.
- A partir do arquivo *platformio.ini*, o PlatformIO fará download e configurará todas as dependências do projeto.
- Se o build terminar com mensagem de sucesso, basta fazer o upload para o ESP32

### Upload para o ESP32

Após o Build ter concluído com sucesso, basta realizar o upload para o ESP32

- Plugue o ESP32 em algum USB
- Clique no ícone de seta na barra de tarefas inferior
- O programa será compilado para uma versão mais otimizada e o upload será realizado para a placa.

### Monitoramento Serial

Já com o programa dentro do ESP32, é possível verificar os "prints" do programa.

- Clique no ícone de tomada na barra de tarefas inferior
- Com o terminal aberto, o programa agora mostrará todos os Serial.print no terminal


<!-- - em seguida, já com o projeto clonado na sua máquina, abra o arquivo ```.vscode/c_cpp_properties.json``` e altere todas as referências de nome de usuário para o nome do usuário do seu computador (no momento esta como "breno". Então, basta mudar para o nome do seu usuário;
- em seguida pegue tudo dentro da pasta "libraries" do projeto e cole na pasta ```Documentos/Arduino/libraries```
- em seguida, aperte F1 para abrir a paleta de comando, digite "Arduino" e clique no comando "Board Manager" e instale o pacote "esp32 by Espressif Systems Version". Por fim, instale a 
- por último, você pode reparar que, talvez, a linha no arquivo ```.vscode/c_cpp_properties.json``` chamada ```compilerPath```pode estar acusando um erro. Para resolver isso, vá para o mesmo caminho, até logo antes 3 últimos endereços. Por exemplo: com o caminho para ```C:\Users\breno\AppData\Local\Arduino15\packages\esp32\tools\xtensa-esp32-elf-gcc\1.22.0-97-gc752ad5-5.2.0\bin\xtensa-esp32-elf-g++``` dando erro, vá até ```C:\Users\breno\AppData\Local\Arduino15\packages\esp32\tools\xtensa-esp32-elf-gcc\```. Por fim, clique na pasta que aparece na tela, que vai ter o formato do nome parecido com o anterior e substituia o final do caminho por esse novo caminho encontrado. -->

<!-- ## Como Utilizar
---

O Programa possui algumas variáveis para realizar o controle e também conta com uma pequena API para poder compartilhar informações sobre o estado da caixa/cisterna, e até receber comandos.

### Variáveis

- **Full:** Diz se a caixa/cisterna está cheia.
- **Empty:** Diz se a caixa/cisterna está vazia.
- **Locked:** Utilizado para bloquear o sistema na caixa ou na cisterna.
- **Water Out:** Diz se está jogando água para algum lugar. No caso da cisterna para a caixa d'água.
- **Water In:** Diz se está está recebendo água de algum lugar. No caso, se a caixa está sendo enchida, ou precisa ser enchida.

### Constantes
- ```SYSTEM_APPLICATION_TYPE```: Declara o tipo de aplicação da placa, *Caixa d'água* ou *Cisterna*.
- ```SENSOR_HEIGHT```: Declara a altura do ponto onde o sensor ultrassônico está posicionado até o fundo da caixa/cisterna, em centímetros.
- ```FULL_LEVEL_PERCENTAGE```: Declara a porcentagem máxima de água, em relação a altura do sensor, para a caixa/cisterna ser considerada **CHEIA**.
- ```EMPTY_LEVEL_PERCENTAGE```: Declara a porcentagem mínima de água, em relação a altura do sensor, para a caixa/cisterna ser considerada **VAZIA**.


### Pinagem

| Pino | Propósito |
| :-: | :-: |
| 13 | Trigger (Sensor ultrassônico) |
| 12 | Echo (Sensor ultrassônico) |
| 25 | Water Out (Para ligar no relé) |

### Configurando

1. Para realizar a configuração, é necessário medir a altura a partir da saída do auto-falante do sensor até o fundo da caixa/cisterna e colocar o valor, em centímetros, na variável ```SENSOR_HEIGHT```. Após isso informar a porcentagem onde a caixa/cisterna será considerada ***cheia*** ou ***vazia*** e declarar o valor nas variáveis ```FULL_LEVEL_PERCENTAGE``` e ```EMPTY_LEVEL_PERCENTAGE``` respectivamente.
1. Alimente o sensor com a placa através do uso do pino 3v3 (não é necessário o uso de resistores), e em seguida ligue os pinos ***Trigger*** e ***Echo***.
1. Caso a placa seja a configurada para a cisterna, ligue o pino ***Water Out*** no Relé.
1. Na seção de **configuração do WiFi** insira os dados da rede para que as placas consigam se comunicar em LAN. É muito importante que as placas estejam conectadas na **MESMA REDE**. <br>
As placas ainda não conseguem se conectar com IP estático na rede, então deve-se consultar no modem da Internet pelos dispositivos conectados para saber o IP deles, ou, faça testes utilizando a saída do Serial em um computador e veja as informações da rede que a placa mostra na tela. -->
