-- Autor reseni: Monika Rosinska xrosin00

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.
	SMCLK : IN std_logic;
	RESET : IN std_logic;
	ROW : OUT std_logic_vector(0 to 7); --signaly - radek matice
	LED : OUT std_logic_vector(0 to 7) --signaly - sloupec matice
	);
end ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.
	 signal state : std_logic := '0'; --povolovaci signal
	 signal state_cnt : std_logic_vector(7 downto 0) := (others => '0'); --signal pro generovani povolovaciho signalu (stridani radku)
	 signal switch : std_logic_vector(22 downto 0) := (others => '0'); --signal na zhasnuti diod po 0.5 s (delka 7 372 800/2 = 3 686 400 v bin)
	 signal rows : std_logic_vector(0 to 7) := "10000000"; --signal pro rows (nastavene na prvni radek)
	 signal leds : std_logic_vector(0 to 7) := (others => '1'); --signal pro leds (vsechny jsou zhasnute)
	 
	 
	 --deleni frekvence: 7372800/256/8 (8 radku)
	 begin
	 	 freq_low: process(SMCLK, RESET)
		 begin
			if RESET = '1' then
				state_cnt <= (others => '0');
			elsif SMCLK' event and SMCLK = '1' then
				state_cnt <= state_cnt + 1;
			end if;
		end process freq_low;
		state <= '1' when state_cnt = X"FF" else '0';
		
		 switch_off: process(SMCLK, RESET, switch) --off
			begin
				if RESET = '1' then
					switch <= (others => '0');
				elsif SMCLK' event and SMCLK = '1' then
					switch <= switch + 1;
				end if;
			if switch = "11111111111111111111111" then
				switch <= "11100001000000000000000";
			else
			end if;
			end process switch_off;

	 -- Sem doplnte popis obvodu. Doporuceni: pouzivejte zakladni obvodove prvky
    -- (multiplexory, registry, dekodery,...), jejich funkce popisujte pomoci
    -- procesu VHDL a propojeni techto prvku, tj. komunikaci mezi procesy,
    -- realizujte pomoci vnitrnich signalu deklarovanych vyse.

    -- DODRZUJTE ZASADY PSANI SYNTETIZOVATELNEHO VHDL KODU OBVODOVYCH PRVKU,
    -- JEZ JSOU PROBIRANY ZEJMENA NA UVODNICH CVICENI INP A SHRNUTY NA WEBU:
    -- http://merlin.fit.vutbr.cz/FITkit/docs/navody/synth_templates.html.

    -- Nezapomente take doplnit mapovani signalu rozhrani na piny FPGA
    -- v souboru ledc8x8.ucf.
		rows_rotation: process(SMCLK, RESET, state)
		begin
			if RESET = '1' then
				rows <= "10000000"; --nastavi signal opet na prvni radek
			elsif SMCLK' event and SMCLK = '1' then
				if state = '1' then
					rows <= (rows(7) & rows(0 to 6)); --posunuti na dalsi radek				
				else
				end if;
			end if;
		end process rows_rotation;
		
		ROW <= rows; --nastaveni radku na aktualni signaly
		
		activate_leds: process(rows, switch) --off
		begin
			if switch < "01110000100000000000001" or switch > "11100001000000000000000" then
				case rows is
					when "10000000" => leds <= "01110111";
					when "01000000" => leds <= "00100111";
					when "00100000" => leds <= "01010111";
					when "00010000" => leds <= "01110000";
					when "00001000" => leds <= "01110110";
					when "00000100" => leds <= "11110000";
					when "00000010" => leds <= "11110101";
					when "00000001" => leds <= "11110110";
					when others => leds <= (others => '1'); --zadna ledka nesviti
				end case;
			else
				leds <= (others => '1'); 
			end if;
		end process activate_leds;
		
		LED <= leds; --nastaveni ledek na aktualni signaly

end main;




-- ISID: 75579
