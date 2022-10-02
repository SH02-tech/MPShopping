/* 
 * File:   matrix.cpp
 * Author: MP-team 
 * @warning To be implemented by students. Follow the instructions given in the headers
 */


#include "Matrix.h"
#include <fstream>
#include <assert.h>
#include <iomanip>

using namespace std;

double v_npos = Matrix::npos;
const double c_npos = Matrix::npos;

/**
 * @brief Copies the values in a 2D matrix org with nrows and ncols to an also 2D matrix dest.
 * It is assumed that org and dest have the memory properly allocated 
 * @param dest destination matrix
 * @param org  source matrix
 * @param nrows number of rows
 * @param ncols number of cols
 */
void copy(double **dest, double **org, size_t nrows, size_t ncols) {
    for (int i=0; i<nrows; i++)
        for (int j=0; j<ncols; j++)
            dest[i][j] = org[i][j];
}

/** 
 * @brief this method reserve memory to allocate a 2D matrix of size r x c.
 * @param r number of rows
 * @param c number of cols
 * @return the pointer to the memory block containing the data
 */
double ** allocate(size_t r, size_t c) {
    double ** block;
    // allocate memory into block
    
    block = nullptr;
    
    if (r>0 && c>0) {
        block = new double*[r];

        for (int i=0; i<r; i++)
            block[i] = new double[c];
    } 
    
    return block;
}

/**
 * @brief In this method, given an index idx it returns the key in the position i, 
 * such that i is the first position in the index such that idx.at(i).getPos()==p
 * @param idx the input index
 * @param p number of 
 * @return the key for first position i in the index such that idx.at(i).getPos()==p
 */
string labelWithValuePosInIndex(const Index & idx, size_t p) {
    string salida;

    bool enc = false;
    for (int i = 0; i < idx.size() && !enc; i++) {
        if (idx.at(i).getPos() == p) {
            salida = idx.at(i).getKey();
            enc = true;
        }
    }
    return salida;
}

void Matrix::deallocate() {
    if (_data != nullptr) {
        for (int i=0; i<rows(); i++) {
            if (_data[i] != nullptr) {
                delete[] _data[i];
                _data[i] = nullptr;
            }
        }
        delete[] _data;
        _data = nullptr;
    }
    _nrows = 0;
    _ncols = 0;
    _rowLabels.clear();
    _colLabels.clear();
}


Matrix::Matrix() {
   _data = nullptr;
   clear();
}

void Matrix::setValues(double valor) {
    for (int i=0; i<_nrows; i++)
        for (int j=0; j<_ncols; j++)
            _data[i][j] = valor;
}

Matrix::Matrix(size_t f, size_t c, double vdef) {
    int rows = (f > 0 ? f : 0);
    int cols = (c > 0 ? c : 0);
    
    _data = nullptr;
    resize(rows, cols, vdef);
}

Matrix::Matrix(const Matrix& orig) {
    _data = nullptr;
    clear();
    
    string rowLabels[orig.rows()], colLabels[orig.columns()];
    orig.getRowLabels(rowLabels);
    orig.getColLabels(colLabels);
    
    resize(rowLabels, orig.rows(), colLabels, orig.columns());
    copy(_data, orig._data, _nrows, _ncols);
}

Matrix::Matrix(const std::string *rowLabels, size_t nrows, const std::string * colsLabels, size_t ncols, double value) {
    _data = nullptr;
    resize(rowLabels, nrows, colsLabels, ncols, value);
}

std::string Matrix::labelAtRow(size_t row) const {
    return labelWithValuePosInIndex(_rowLabels, row);
}

std::string Matrix::labelAtCol(size_t col) const {
    return labelWithValuePosInIndex(_colLabels, col);
}

void Matrix::resize(size_t nrows, size_t ncols, double value) {
    clear();
    _data = allocate(nrows, ncols);
    _nrows = nrows;
    _ncols = ncols;
    setValues(value);
}

void Matrix::resize(const std::string * rowLabels, size_t nrows, const std::string * colLabels, size_t ncols, double value) {
    resize(nrows, ncols, value);    
    setLabels(_rowLabels, rowLabels, nrows);
    setLabels(_colLabels, colLabels, ncols);
}

void Matrix::clear() {
    deallocate();
}

Matrix::~Matrix() {
    clear();
}

const double & Matrix::at(size_t f, size_t c) const {
    if (0<=f && f<rows() && 0<=c && c<columns())
        return _data[f][c];
    else {
        return v_npos;
    }
}

double & Matrix::at(size_t f, size_t c) {
    if (0<=f && f<rows() && 0<=c && c<columns())
        return _data[f][c];
    else {
        return v_npos;
    }
}

double & Matrix::operator()(size_t f, size_t c) {
    return at(f,c);
}

const double & Matrix::operator()(size_t f, size_t c) const {
    return at(f,c);
}

int Matrix::rowIndexOf(const std::string & label) const {
    int pos = npos;
    bool found = false;
    
    for (int i=0; i<rows() && !found; i++)
        if (_rowLabels.at(i).getKey() == label) {
            pos = i;
            found = true;
        }
    
    return pos;
}

int Matrix::colIndexOf(const std::string & label) const {
    int pos = npos;
    bool found = false;
    
    for (int i=0; i<columns() && !found; i++)
        if (_colLabels.at(i).getKey() == label) {
            pos = i;
            found = true;
        }
    
    return pos;
}

double & Matrix::at(const std::string & rowLabel, const std::string & colLabel) {
    int row = rowIndexOf(rowLabel);
    int col = colIndexOf(colLabel);
    
    return at(row, col);
}

double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel) {
    return at(rowLabel, colLabel);
}

const double & Matrix::at(const std::string & rowLabel, const std::string & colLabel) const {
    int row = rowIndexOf(rowLabel);
    int col = colIndexOf(colLabel);
    
    return at(row, col);
}

const double & Matrix::operator()(const std::string & rowLabel, const std::string & colLabel)const {
    return at(rowLabel, colLabel);
}

ostream & operator<<(ostream & flujo, const Matrix& m) {
    const int WIDTH = 5;
    
    const int ROWS = m.rows();
    const int COLS = m.columns();
        
    flujo << ROWS << " " << COLS << "\n";
    
    for (int i=0; i<ROWS; i++)
        flujo << m._rowLabels.at(i).getKey() << setw(WIDTH) << "\t";
    
    flujo << "\n";
    
    for (int j=0;j<COLS; j++)
        flujo << m._colLabels.at(j).getKey() << setw(WIDTH) << "\t";
    
    flujo << "\n";
    
    for (int i=0; i<ROWS; i++) {
        
        for (int j=0; j<COLS; j++)
            flujo << setprecision(5) << (double)m(i,j) << setw(WIDTH) << "\t";
        
        flujo << "\n";
    }
    
    flujo << "\n";
    
    return flujo;    
}

Matrix & Matrix::operator=(const Matrix& m) {    
    if (m._data != this->_data) {
        this->clear();
        
        if (m._data != nullptr) {
            string rowLabels[m.rows()], colLabels[m.columns()];
            m.getRowLabels(rowLabels);
            m.getColLabels(colLabels);

            this->resize(rowLabels, m.rows(), colLabels, m.columns());
            copy(this->_data, m._data, _nrows, _ncols);
        }
    }
    
    return *this;
}

bool Matrix::setLabels(Index & idx, const string * labels, int nlabels){
    idx.clear();
    int lb,ub;
    bool correct = true;
    for (int i=0; i< nlabels && correct; i++){
        lb = idx.lower_bound(labels[i]);
        ub = idx.upper_bound(labels[i]);
        if (lb==ub) { // it is NOT in the set
            Pair one(labels[i], i);
            idx.add(one);
        } else {
            correct = false;
         }
    }
    if (!correct) idx.clear();
    
    return correct;
}

int Matrix::getLabels(const Index & idx, string * labels) const {
    if (idx.size() > 0) {
        for (int i = 0; i < idx.size(); i++) {
            labels[i] = idx.at(i).getKey();
        }
    }
    return idx.size();
}

int Matrix::getRowLabels(string * labels) const {
    return getLabels(_rowLabels,labels);
}

int Matrix::getColLabels(string *labels) const {
    return getLabels(_colLabels,labels);
}

size_t Matrix::columns() const {
    return _ncols;
}

size_t Matrix::rows() const {
    return _nrows;
}

void Matrix::save(const string & nf) const{
    ofstream fsal(nf);
    if (fsal.is_open()) {
        fsal << "#matrix "; 
        if (_rowLabels.size()==0)
            fsal << "unlabeled"<< endl;
        else fsal << "labeled" << endl;
        fsal << *this;
        fsal.close();
    } else cerr << "Fail creating " << nf << " file" <<endl;
}

void Matrix::load(const string & nf) {
    ifstream fent(nf);
    string labeled, cad;
    clear();
    if (fent.is_open()) {
         fent >> cad >> labeled;
         fent >> _nrows >> _ncols;
         if (labeled == "labeled"){
            for (int i=0;i<_nrows;i++){
                fent >> cad;
                _rowLabels.add(Pair(cad,i));
            }
             for (int i=0;i<_ncols;i++){
                fent >> cad;
                _colLabels.add(Pair(cad,i));
            }
        }
        _data = allocate(_nrows,_ncols); 
        for (int i = 0; i < _nrows; i++) {
            for (int j = 0; j < _ncols; j++) {
                fent >> _data[i][j];
            }
        }
        fent.close();
    } else cerr << "Fail reading " << nf << " file" <<endl;
}

Matrix & Matrix::operator*=(double val) {
    for (int i=0; i<this->rows(); i++)
        for (int j=0; j<this->columns(); j++)
            this->at(i,j) *= val;
    
    return *this;
}

Matrix & Matrix::operator+=(double val) {
    for (int i=0; i<this->rows(); i++)
        for (int j=0; j<this->columns(); j++)
            this->at(i,j) += val;
    
    return *this;
}

Matrix Matrix::operator*(const Matrix & m) const {
    Matrix matrix_product;
    
    if (this->columns() == m.rows()) {
        string rowLabels[this->_rowLabels.size()];
        string colLabels[m._colLabels.size()];
        
        int numRowLabels = getRowLabels(rowLabels);
        int numColLabels = m.getColLabels(colLabels);
        
        if (numRowLabels != 0 && numColLabels != 0)
            matrix_product.resize(rowLabels, this->rows(), colLabels, m.columns());
        else
            matrix_product.resize(this->rows(), m.columns());
        
        for (int i=0; i<matrix_product.rows(); i++)
            for (int j=0; j<matrix_product.columns(); j++)
                for (int k=0; k<this->columns(); k++)
                    matrix_product(i,j) += this->at(i,k) * m.at(k,j);
    } else {
        matrix_product.resize(3,3);
        matrix_product.setValues(npos);
    }
    
    return matrix_product;
}

Matrix Matrix::operator+(double val) const {
    Matrix sum(*this);
    
    sum += val;
    
    return sum;
}

Matrix Matrix::operator*(double val) const {
    Matrix product(*this);
    
    product *= val;
    
    return product;
}

Matrix operator+(double ival, const Matrix & dch) {
    Matrix sum(dch);
    
    sum += ival;
    
    return sum;
}

Matrix operator*(double ival, const Matrix & dch) {
    Matrix product(dch);
    
    product *= ival;
    
    return product;
}

Matrix & Matrix::normalize(){
    const int ROWS = this->rows();
    const int COLS = this->columns();
    
    for (int i=0; i<ROWS; i++) {
        double sum_row = 0;
        
        for (int j=0; j<COLS; j++)
            sum_row += this->at(i,j);
        
        double refactor = (sum_row != 0 ? 1.0/sum_row : 0);
        
        if (refactor != 0)
            for (int j=0; j<COLS; j++)
                this->at(i,j) *= refactor;
        else
            for (int j=0; j<COLS; j++)
                this->at(i,j) = 1.0/COLS;
    }
    
    return *this;
}
