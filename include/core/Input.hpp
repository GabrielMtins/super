#ifndef INPUT_HPP
#define INPUT_HPP

#include <array>
#include <unordered_map>
#include <bitset>

#include "core/CoreDef.hpp"

#include <SDL2/SDL.h>

class Input {
	public:
		enum Mode {
			MODE_KEYBOARD,
			MODE_GAMEPAD
		};

		Input(void);
		
		/**
		 * Atualiza os inputs dado o tick atual.
		 */
		void update(void);

		/**
		 * Define uma tecla (SDL_Scancode) para o input.
		 */
		void setInputKey(int input, int key);

		/**
		 * Define um botão de gamepad (SDL_CONTROLLER_BUTTON*) para
		 * o input.
		 */
		void setInputButton(int input, int button);

		/**
		 * Retorna verdadeiro caso a tecla esteja pressionada.
		 */
		bool getInput(int input) const;

		/**
		 * Retorna verdadeiro caso a tecla tenha sido pressionada
		 * naquele tick.
		 */
		bool getInputDown(int input) const;

		/**
		 * Retorna verdadeiro caso a tecla tenha sido solta
		 * naquele tick.
		 */
		bool getInputUp(int input) const;

		/**
		 * Se conecta com o controle dado o index.
		 */
		bool openController(int index);

		/**
		 * Termina a conexão com o controle.
		 */
		void closeController(void);

		/**
		 * Define o modo de input.
		 */
		void setMode(Mode mode);
	
	private:
		void updateKeyState(void);
		void updateButtonState(void);
		bool valid(int input) const;

		std::array<int, MAX_INPUT_TYPES> input_to_key;
		std::array<int, MAX_INPUT_TYPES> input_to_button;

		std::bitset<MAX_INPUT_TYPES> pressed;
		std::bitset<MAX_INPUT_TYPES> old_pressed;

		SDL_GameController *controller;
		Mode mode;
};

#endif
