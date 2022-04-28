# Monitoramento de Nível de Água

Projeto feito como Trabalho de Conclusão de curso para a Graduação de Ciência da Computação na Universidade Federal Fluminense.

- Aluno: Breno Reis dos Santos
- Orientador: Raphael Guerra

<br>

# Propósito

O projeto tem como objetivo realizar o monitoramento do nível da água de 1 Cisterna e 4 Caixas d'água no mesmo nível. Esse monitoramento escreve registros em um arquivo que será usado para gerar relatórios mostrando o histórico de abastecimento de água na casa.

<br>

# Como Utilizar

O Programa possui algumas variáveis para realizar o controle e também conta com uma pequena API para poder compartilhar informações sobre o estado da caixa/cisterna, e até receber comandos.

## Variáveis

- **Full:** Diz se a caixa/cisterna está cheia.
- **Empty:** Diz se a caixa/cisterna está vazia.
- **Locked:** Utilizado para bloquear o sistema na caixa ou na cisterna.
- **Water Out:** Diz se está jogando água para algum lugar. No caso da cisterna para a caixa d'água.
- **Water In:** Diz se está está recebendo água de algum lugar. No caso, se a caixa está sendo enchida, ou precisa ser enchida.

## Constantes
- ```SYSTEM_APPLICATION_TYPE```: Declara o tipo de aplicação da placa, *Caixa d'água* ou *Cisterna*.
- ```SENSOR_HEIGHT```: Declara a altura do ponto onde o sensor ultrassônico está posicionado até o fundo da caixa/cisterna, em centímetros.
- ```FULL_LEVEL_PERCENTAGE```: Declara a porcentagem máxima de água, em relação a altura do sensor, para a caixa/cisterna ser considerada **CHEIA**.
- ```EMPTY_LEVEL_PERCENTAGE```: Declara a porcentagem mínima de água, em relação a altura do sensor, para a caixa/cisterna ser considerada **VAZIA**.


## Pinagem

| Pino | Propósito |
| :-: | :-: |
| 13 | Trigger (Sensor ultrassônico) |
| 12 | Echo (Sensor ultrassônico) |
| ? | Water Out (Para ligar no relé) |

## Configurando

1. Para realizar a configuração, é necessário medir a altura a partir da saída do auto-falante do sensor até o fundo da caixa/cisterna e colocar o valor, em centímetros, na variável ```SENSOR_HEIGHT```. Após isso informar a porcentagem onde a caixa/cisterna será considerada ***cheia*** ou ***vazia*** e declarar o valor nas variáveis ```FULL_LEVEL_PERCENTAGE``` e ```EMPTY_LEVEL_PERCENTAGE``` respectivamente.
1. Alimente o sensor com a placa através do uso do pino 3v3 (não é necessário o uso de resistores), e em seguida ligue os pinos ***Trigger*** e ***Echo***.
1. Caso a placa seja a configurada para a cisterna, ligue o pino ***Water Out*** no Relé.
1. Na seção de **configuração do WiFi** insira os dados da rede para que as placas consigam se comunicar em LAN. É muito importante que as placas estejam conectadas na **MESMA REDE**. <br>
As placas ainda não conseguem se conectar com IP estático na rede, então deve-se consultar no modem da Internet pelos dispositivos conectados para saber o IP deles, ou, faça testes utilizando a saída do Serial em um computador e veja as informações da rede que a placa mostra na tela.

# Roadmap

- A forma de utilizar as constantes de gatilho para ***cheio*** e ***vazio*** serão alteradas no futuro.
- Utilização de IP estático. (Configuração atual não está funcionando corretamente)
- Adicionar interações em ambos sentidos entre a placa da caixa e da cisterna
- Implementar a funcionalidade do ***lock*** para travar o funcionamento das placas, de forma individual.
