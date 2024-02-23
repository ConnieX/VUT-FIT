-- fsm.vhd: Finite State Machine
-- Author(s): 
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST_DEFAULT, TEST_DENIED, TEST_1, TEST_12, TEST_12_A_01, TEST_12_A_02, TEST_12_A_03, TEST_12_A_04, TEST_12_A_05, TEST_12_A_06, TEST_12_A_07, TEST_12_A_08, TEST_12_B_01, TEST_12_B_02, TEST_12_B_03, TEST_12_B_04, TEST_12_B_05, TEST_12_B_06, TEST_12_B_07, TEST_12_B_08, PRINT_DENIED, PRINT_ALLOWED, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST_DEFAULT;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_DEFAULT =>
      next_state <= TEST_DEFAULT;
		if (KEY(1)  = '1') then
			next_state <= TEST_1;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_1 =>
      next_state <= TEST_1;
		if (KEY(2)  = '1') then
			next_state <= TEST_12;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12 =>
      next_state <= TEST_12;
		if (KEY(5)  = '1') then
			next_state <= TEST_12_A_01;
		elsif (KEY(2)  = '1') then
			next_state <= TEST_12_B_01;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_01 =>
      next_state <= TEST_12_A_01;
		if (KEY(5)  = '1') then
			next_state <= TEST_12_A_02;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_02 =>
      next_state <= TEST_12_A_02;
		if (KEY(7)  = '1') then
			next_state <= TEST_12_A_03;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_03 =>
      next_state <= TEST_12_A_03;
		if (KEY(2)  = '1') then
			next_state <= TEST_12_A_04;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_04 =>
      next_state <= TEST_12_A_04;
		if (KEY(1)  = '1') then
			next_state <= TEST_12_A_05;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_05 =>
      next_state <= TEST_12_A_05;
		if (KEY(2)  = '1') then
			next_state <= TEST_12_A_06;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_A_06 =>
      next_state <= TEST_12_A_06;
		if (KEY(8)  = '1') then
			next_state <= TEST_12_A_07;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
		when TEST_12_A_07 =>
      next_state <= TEST_12_A_07;
		if (KEY(8)  = '1') then
			next_state <= TEST_12_A_08;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_12_A_08 =>
      next_state <= TEST_12_A_08;
		if (KEY(15)  = '1') then
			next_state <= PRINT_ALLOWED;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_01 =>
      next_state <= TEST_12_B_01;
		if (KEY(6)  = '1') then
			next_state <= TEST_12_B_02;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_02 =>
      next_state <= TEST_12_B_02;
		if (KEY(6)  = '1') then
			next_state <= TEST_12_B_03;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_03 =>
      next_state <= TEST_12_B_03;
		if (KEY(1)  = '1') then
			next_state <= TEST_12_B_04;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_04 =>
      next_state <= TEST_12_B_04;
		if (KEY(6)  = '1') then
			next_state <= TEST_12_B_05;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_05 =>
      next_state <= TEST_12_B_05;
		if (KEY(7)  = '1') then
			next_state <= TEST_12_B_06;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
	when TEST_12_B_06 =>
      next_state <= TEST_12_B_06;
		if (KEY(7)  = '1') then
			next_state <= TEST_12_B_07;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
		when TEST_12_B_07 =>
      next_state <= TEST_12_B_07;
		if (KEY(2)  = '1') then
			next_state <= TEST_12_B_08;
      elsif (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_12_B_08 =>
      next_state <= TEST_12_A_08;
		if (KEY(15)  = '1') then
			next_state <= PRINT_ALLOWED;
		elsif (KEY(14 downto 0) /= "000000000000000") then
			next_state <= TEST_DENIED;
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST_DENIED =>
      next_state <= TEST_DENIED;
      if (KEY(15) = '1') then
         next_state <= PRINT_DENIED; 
      end if; 
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_DENIED =>
      next_state <= PRINT_DENIED;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ALLOWED =>
      next_state <= PRINT_ALLOWED;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST_DEFAULT; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST_DEFAULT;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_DENIED =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_ALLOWED =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
		FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;

