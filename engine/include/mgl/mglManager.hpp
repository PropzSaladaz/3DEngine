#ifndef MGL_MANAGER_HPP
#define MGL_MANAGER_HPP


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <mgl/models/meshes/mglMesh.hpp>
#include <utils/Logger.hpp>

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <typeinfo>

namespace mgl {

	/// <summary>
	/// Represents any manager that manages a set of items stored in a kay-value map
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class Manager {
	public:
		using SetManagedItemCallback = std::function<void(T* item)>;
		Manager();
		~Manager();
		virtual void add(const std::string& name, std::shared_ptr<T> item);
		void remove(const std::string& name);
		T* get(const std::string& name);
		void forEach(SetManagedItemCallback function);

	protected:
		void setManagedItemCallback(SetManagedItemCallback callback);
		SetManagedItemCallback itemCallback;
	private:
		std::map<std::string, std::shared_ptr<T>> items;

	};

	// template class definitions 

	template<typename T>
	Manager<T>::Manager() {}
	template<typename T>
	Manager<T>::~Manager() {}

	template <typename T>
	void Manager<T>::add(const std::string& name, std::shared_ptr<T> item) {
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
			MGL_ERROR("No item found with name " + name + " in manager of " + typeid(T).name());
			exit(EXIT_FAILURE);
		}
	}

	template <typename T>
	void Manager<T>::setManagedItemCallback(SetManagedItemCallback callback) {
		itemCallback = callback;
	}

	template <typename T>
	void Manager<T>::forEach(SetManagedItemCallback function) {
		for (const auto& item : items) {
			function(item.second);
		}
	}


}

#endif