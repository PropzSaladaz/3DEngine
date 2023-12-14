#ifndef MGL_MANAGER_HPP
#define MGL_MANAGER_HPP


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mgl/scene/mglMesh.hpp>
#include <iostream>
#include <string>
#include <map>
#include <functional>

namespace mgl {


	template<typename T>
	class Manager {
	public:
		using SetManagedItemCallback = std::function<void(T* item)>;
		Manager();
		~Manager();
		virtual void add(const std::string& name, T* item);
		void remove(const std::string& name);
		T* get(const std::string& name);

		void setManagedItemCallback(SetManagedItemCallback callback);
	protected:
		SetManagedItemCallback itemCallback;
	private:
		std::map<std::string, T*> items;

	};

	// template class definitions 

	template<typename T>
	Manager<T>::Manager() {}
	template<typename T>
	Manager<T>::~Manager() {}

	template <typename T>
	void Manager<T>::add(const std::string& name, T* item) {
		items.insert(std::make_pair(name, item));
	}

	template <typename T>
	void Manager<T>::remove(const std::string& name) {
		items.erase(name);
	}

	template <typename T>
	T* Manager<T>::get(const std::string& name) {
		auto it = items.find(name);
		if (it != items.end()) {
			return it->second;
		}
		else {
			std::cerr << "MESH_MANAGER::GET::ERROR" << std::endl;
			std::cerr << "No mesh found with name " << name << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	template <typename T>
	void Manager<T>::setManagedItemCallback(SetManagedItemCallback callback) {
		itemCallback = callback;
	}
}

#endif