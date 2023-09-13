# Projeto-Monitoramento-de-Ambientes-Inteligentes


<h2>Manual de operação</h2>
                                                                             
O equipamento tem um funcionamento bem simples, basta conecta-lo a uma fonte de energia e aguardar o período de análise.
O equipamento realiza a medição dos sensores a cada 5 segundos, e após 1 minuto calcula a média das leituras realizadas.
Se o valor obtido estiver fora dos parâmetros especificadas, será acionado um alarme sonoro e luminoso, bem como a gravação das medições, junto com o horário exato da leitura na memória do equipamento.
Caso as leituras estejam dentro dos parâmetros informados, nenhuma ação será realizada. 
O funcionamento descrito acima é continuo, e o monitoramento será realizado enquanto o equipamento estiver ligado.


<h2>Especificações tecnicas</h2>

Essa PoC (Proof of Concept) consiste em um medidor de valores automatizado para o disparo de alertas sonoro e luminoso, caso ultrapasse as condições previamente parametrizadas. O sistema, apresenta as leituras no display de cristal líquido (LCD) e realiza o backup na memória (EEPROM - data logger) com o timestamp. O mesmo pode ser tranportado pois é alimentado através de uma bateria.

Especificações de Variáveis:
  
  Temperatura: 15 < t < 25 ºC
  
  Luminosidade: 0 < l < 30%
  
  Umidade: 30% < u < 50%

<h3>Lista de Materiais e Componentes:</h3>

 - 1 MCU (Atmega 328P) - Arduino Uno R3

 - 1 LDR + Resistor 10KOhm

 - 1 DHT-11 (Sensor de temperatura e umidade)

 - 1 LCD 16x2

 - 1 Bateria de 9V

 - 1 RTC (Real Time Clock)

 - 1 protoboard

 - jumpers

 - LEDs

 - Resistores

 - 1 Suporte para bateria


<h2>Diagrama Elétrico:</h2>

![WhatsApp Image 2023-09-11 at 21 49 02](https://github.com/vinascim/Projeto-Monitoramento-de-Ambientes-Inteligentes/assets/69062237/0555a24e-7c90-44fc-a98c-0d363aa00640)


<h2>Video explicando o projeto:</h2>
https://youtu.be/A45tleK9Tzk?si=o-kU6tFB0UQpKVgr

<h2>Video em timelapse:</h2>
https://youtube.com/shorts/PahZcUJ7eOo?feature=share
